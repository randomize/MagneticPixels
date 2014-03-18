#include "Strategies.h"

#include "Context.h"
#include "Pixel.h"


using namespace MPix;

//====---------------------- Alive -----------------------======//

EM_NODE_CHILD_CPP(AliveImmortal);
bool MPix::AliveImmortal::canLive( shared_ptr<IAlive> subject, const Context& context )
{
   return true;
}

EM_NODE_CHILD_CPP(AliveSimple);

bool MPix::AliveSimple::GotAnyKillerAt( int subject, const Context& context, Coordinates pos )
{
   bool ret = false;

   forward_list<shared_ptr<IKilling>> killers;
   context.GetPixelsAt(killers, pos);
   for ( auto k : killers ) {
      if ( k->tryKillThat(context, subject)) {
         ret = true;
      }
   }
   return ret;
}

bool MPix::AliveSimple::canLive( shared_ptr<IAlive> subject, const Context& context )
{
   if (subject->IsAlive() == false) {
      context.PostEvent(PixelEvent::DIED, subject->GetID());
      return false;
   }
   // Check my pos
   return GotAnyKillerAt(subject->GetID(), context, subject->GetPos()) == false;
}

EM_NODE_CHILD_CPP(AliveStandard);
bool MPix::AliveStandard::canLive( shared_ptr<IAlive> subject, const Context& context )
{
   // Check my pos
   bool ret = AliveSimple::canLive(subject, context);

   // Plus check 4 major side pos
   for (auto d : EnumRanger<Direction>(DirectionType::MAJOR)) {
      auto p = subject->GetPos() + d;
      if ( GotAnyKillerAt(subject->GetID(), context, p) ) {
         ret = false;
      }
   }
   return ret;
}

EM_NODE_CHILD_CPP(AliveExtended);
bool MPix::AliveExtended::canLive( shared_ptr<IAlive> subject, const Context& context )
{
   // Check major and my pos
   bool ret = AliveStandard::canLive(subject, context);

   // Plus check 4 minor pos
   for (auto d : EnumRanger<Direction>(DirectionType::MINOR)) {
      auto p = subject->GetPos() + d;
      if ( GotAnyKillerAt(subject->GetID(), context, p) ) {
         ret = false;
      }
   }
   return ret;
}

//====---------------------- Killers -----------------------======//

EM_NODE_CHILD_CPP(KillingBase);
void MPix::KillingBase::PerformKill( const IKilling& subject, const Context& context, IAlive& victim )
{
   // Kill
   if (victim.IsAlive()) {
      victim.Kill(context, subject);
   }
   // Smile
   if (victim.KilledBy(subject)) { // That was me(my type)
      context.PostEvent(PixelEvent::KILLED, subject.GetID());
   }
}

bool MPix::KillingBase::tryKillThat( shared_ptr<IKilling> subject, const Context& context, int target )
{
   // Take victim
   auto px = context.GetByID(target);

   // Assure that is IAlive
   auto victim = std::dynamic_pointer_cast<IAlive> ( px );
   if ( victim == nullptr ) {
      return false;
   }

   PerformKill(*subject, context, *victim);
   return true;
}

EM_NODE_CHILD_CPP(KillingKind);

EM_NODE_CHILD_CPP(KillingAtPoint);
bool MPix::KillingAtPoint::tryKillThat( shared_ptr<IKilling> subject, const Context& context, int target )
{
   // Take victim
   auto px = context.GetByID(target);

   // Assure that is IAlive
   auto victim = std::dynamic_pointer_cast<IAlive> ( px );
   if ( victim == nullptr ) {
      return false;
   }

   // kill only on my pos
   if ( victim->GetPos() != subject->GetPos()) { 
      return false;
   }

   // if it is killable, then measure ranks
   auto kll_victim = std::dynamic_pointer_cast<IKilling>(px);
   if (kll_victim && kll_victim->getKillerRank() >= subject->getKillerRank()) {
      return false; // no kill by rank and higher
   }

   PerformKill(*subject, context, *victim);
   return true;
}

EM_NODE_CHILD_CPP(KillingPitfall);
bool MPix::KillingPitfall::tryKillThat( shared_ptr<IKilling> subject, const Context& context, int target )
{
   // Take victim
   auto px = context.GetByID(target);

   // Assure that is IAlive
   auto victim = std::dynamic_pointer_cast<IAlive> ( px );
   if ( victim == nullptr ) {
      return false;
   }

   // kill only on my pos
   if ( victim->GetPos() != subject->GetPos()) { 
      return false;
   }

   // Check if it is assembly
   auto asm_victim = dynamic_pointer_cast<IAssembled> ( px );
   if (asm_victim && asm_victim->IsInAssembly()) {
      asm_victim->HintCanFall(); // Don't kill but warn assembly
      return false;
   }

   PerformKill(*subject, context, *victim);
   return true;
}


EM_NODE_CHILD_CPP(KillingSpikes);

// Helper for fast list check
bool MPix::KillingSpikes::CheckProtectionAt(  const Context& context, Coordinates pos,shared_ptr<IAlive> victim, shared_ptr<IKilling> killer )
{
      forward_list<shared_ptr<IProtector>> protectors;
      context.GetPixelsAt(protectors, pos);
      for (auto p : protectors ) {
         if (p->isProtecting(context, victim, killer))
            return true;
      }
      return false;
}

bool MPix::KillingSpikes::tryKillThat( shared_ptr<IKilling> subject, const Context& context, int target )
{

   // Take victim
   auto px = context.GetByID(target);

   // Assure that is IAlive
   auto victim = std::dynamic_pointer_cast<IAlive> ( context.GetByID(target) );
   if ( victim == nullptr ) {
      return false;
   }

   // if it is killable, then measure ranks
   auto kll_victim = std::dynamic_pointer_cast<IKilling>(px);
   if (kll_victim && kll_victim->getKillerRank() >= subject->getKillerRank()) {
      return false; // no kill by rank and higher
   }

   auto vpos = victim->GetPos();
   auto mpos = subject->GetPos();
   if ( vpos != mpos ) { // kill anyway on its pos

      // Searching URLD major
      Direction my = Direction::DIR_UNKNOWN;
      for(auto d : EnumRanger<Direction>(DirectionType::MAJOR)) {
         if ( mpos + d == vpos ) {
            my = d; // victim is to this dir form me
            break;
         }
      }
      if ( my == Direction::DIR_UNKNOWN ) // Victim is inaccessible
         return false;

      // Check if victim has protection
      if ( CheckProtectionAt(context, mpos, victim, subject) || 
            CheckProtectionAt(context, vpos, victim, subject) )
            return false;

   }

   PerformKill(*subject, context, *victim);
   return true;
}


//====-------------------- Steppers -------------------------======//

EM_NODE_CHILD_CPP(StepperAlways);
bool MPix::StepperAlways::canMoveThis(shared_ptr<IMovableStepper> subject,  const Context& context, Direction dir )
{
   return false;
}

EM_NODE_CHILD_CPP(StepperStandard);
bool StepperStandard::IsSomeoneBlockingMeAt(shared_ptr<IMovableStepper> subject, const Context& context, Coordinates pos, Direction dir)
{
   forward_list<shared_ptr<IMoveBlocker>> blockers;
   context.GetPixelsAt(blockers, pos);

   bool block = false;
   for (auto b : blockers ) {

      // Skip self
      if (b->GetID() == subject->GetID()) 
         continue;

      if (b->blocksMoves(context, subject->GetPos(), dir, context.GetByID(subject->GetID()))) {
         block = true;
      }
   }

   return block;
}

bool MPix::StepperStandard::canMoveThis(shared_ptr<IMovableStepper> subject,  const Context& context, Direction dir )
{
   bool can_move = true;

   // Calculate new position
   auto new_p = subject->GetPos() + dir;

   // check new pos itself
   can_move = can_move && !IsSomeoneBlockingMeAt(subject, context, new_p, dir);

   // check all possible aliases 8 types, 
   // do not optimize, because need feedback from all blockers, not only first
   for (auto d : EnumRanger<Direction>(DirectionType::ALL) ) {
      can_move = can_move && !IsSomeoneBlockingMeAt(subject, context, new_p+d, dir);
      //EM_LOG_ERROR("checked"+d);
   }

   return can_move;
}

EM_NODE_CHILD_CPP(StepperHard);
bool MPix::StepperHard::IsSomeoneBlockingMeAt( shared_ptr<IMovableStepper> subject, const Context& context, Coordinates pos, Direction dir )
{
   forward_list<shared_ptr<IMoveBlocker>> blockers;
   context.GetPixelsAt(blockers, pos);

   bool block = false;
   for (auto b : blockers ) {

      if (b->GetType() != Pixel::PixelType::WALL_PIX &&
          b->GetType() != subject->GetType() ) // Not wall and not me
         continue;

      // Skip self
      if (b->GetID() == subject->GetID()) 
         continue;

      if (b->blocksMoves(context, subject->GetPos(), dir, context.GetByID(subject->GetID()))) {
         block = true;
      }
   }

   return block;

}


EM_NODE_CHILD_CPP(StepperMagnetic);
bool MPix::StepperMagnetic::IsSomeoneBlockingMeAt( shared_ptr<IMovableStepper> subject, const Context& context, Coordinates pos, Direction dir )
{
   forward_list<shared_ptr<IMoveBlocker>> blockers;
   context.GetPixelsAt(blockers, pos);

   bool block = false;
   for (auto b : blockers ) {

      auto as = dynamic_pointer_cast<IAssembled>(b);
      if (as && as->IsInAssembly())
         continue;

      // Skip self
      if (b->GetID() == subject->GetID()) 
         continue;

      if (b->blocksMoves(context, subject->GetPos(), dir, context.GetByID(subject->GetID()))) {
         block = true;
      }
   }

   return block;
}


EM_NODE_CHILD_CPP(StepperGhost);
bool MPix::StepperGhost::canMoveThis( shared_ptr<IMovableStepper> subject, const Context& context, Direction dir )
{
   return true;
}

//====-------------------- Move blocker -------------------------======//

EM_NODE_CHILD_CPP(MoveBlockerSimple);
bool MPix::MoveBlockerSimple::blocksMoves( shared_ptr<IMoveBlocker> subject, const Context& context, Coordinates from, Direction to, shared_ptr<Pixel> pix )
{
   auto pixel_new_pos = from + to;
   auto mypos = subject->GetPos();

   if (mypos == pixel_new_pos) {
      return true;
   }

   // Check diagonals
   if (!DirectionIsMajor(to))
   {
      auto ds = SplitDirection(to);
      for ( auto d : ds ) {
         if (pixel_new_pos == mypos + d) {
            return true;
         }
      }
   }

   return false;
}

EM_NODE_CHILD_CPP(MoveBlockerMyType);
bool MPix::MoveBlockerMyType::blocksMoves( shared_ptr<IMoveBlocker> subject, const Context& context, Coordinates from, Direction to, shared_ptr<Pixel> pix )
{
   if (!pix) return false; // No affect on unspecified type

   if ( subject->GetType() != pix->GetType() ) { 
      // Misc types - no affect
      return false;
   }
   
   return MoveBlockerSimple::blocksMoves(subject, context, from, to, pix);
}


//====-------------------- Growwer  -------------------------======//

EM_NODE_CHILD_CPP(DisabledGrow);
bool MPix::DisabledGrow::growThis( shared_ptr<IAssembled> subject, const Context& context )
{
   return false;
}

EM_NODE_CHILD_CPP(MagneticGrowStandard);
shared_ptr<IAssembled> MPix::MagneticGrowStandard::PerformMagneticGrow( shared_ptr<IAssembled> subject, const Context& context, Coordinates pos )
{
   // Get all assembled, then seed not in assembly, and then try cast magnetic
   forward_list<shared_ptr<IAssembled>> asm_pixels;
   context.GetPixelsAt(asm_pixels, pos);

   // Try to magnetize not in assembly
   for (auto p: asm_pixels) {

      // Skip assembled already
      if (p->IsInAssembly())
         continue;

      // Try to use magnetism
      auto mag = dynamic_pointer_cast<IMagnetic>(p);
      if (mag) {
         return mag->tryMagnetizeToMe(context, context.GetByID(subject->GetID()));
      }

   }
   return nullptr;
}

bool MPix::MagneticGrowStandard::growThis( shared_ptr<IAssembled> subject, const Context& context )
{
   bool grow_happened = false;
   for (auto d : EnumRanger<Direction>(DirectionType::MAJOR) ) { 
      auto target = PerformMagneticGrow(subject, context, subject->GetPos() + d);
      if (target) {
         context.AddToAssembly(target);
         grow_happened = true;
      }
   }
   return grow_happened;
}


EM_NODE_CHILD_CPP(MagneticGrowExtended);
bool MPix::MagneticGrowExtended::growThis( shared_ptr<IAssembled> subject, const Context& context )
{
   bool grow_happened = false;
   for (auto d : EnumRanger<Direction>(DirectionType::ALL) ) { 
      auto target = PerformMagneticGrow(subject, context, subject->GetPos() + d);
      if (target) {
         context.AddToAssembly(target);
         grow_happened = true;
      }
   }
   return grow_happened;
}

EM_NODE_CHILD_CPP(CactusWallProtector);
bool MPix::CactusWallProtector::isProtecting( shared_ptr<IProtector> subject, const Context& context, shared_ptr<IAlive> victim, shared_ptr<IKilling> killer )
{
   //auto w = dynamic_pointer_cast<IWallPixel>
   // TODO:
   return true;
}
