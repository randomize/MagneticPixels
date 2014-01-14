#include "PixelView.h"
#include "Pixel.h"
#include "Shake.h"

#include "GameplayManager.h"
#include "GameStateManager.h"

using namespace MPix;

MPix::PixelView::PixelView()
    :View()
{
}

MPix::PixelView::~PixelView()
{
   // Default unbinding - remove contents if any
   if (target == nullptr) return;
   if (contents != nullptr) {
      target->removeChild(contents);
   }
}

shared_ptr<PixelView> MPix::PixelView::create( shared_ptr<Pixel> px )
{
   string name = px->GetTypeName() + "view";
   shared_ptr<PixelView> view(dynamic_cast<PixelView*>(PixelView::Factory()->NewNodeByName(name)));
   assert(view);
   return view;
}

void MPix::PixelView::Build( shared_ptr<Pixel> model )
{
   assert(model);
   assert(!contents);
   contents = Node::create();
   contents->setPosition(LogicToScreen(model->GetPos())+MPIX_CELL_SIZE_HALF_P);
   this->model = model;
}

bool MPix::PixelView::Update( CmdUIUpdatePixelView::Reason reason )
{
   switch (reason)
   {
   case MPix::CmdUIUpdatePixelView::Reason::CREATED:
      PixelCreated();
      break;
   case MPix::CmdUIUpdatePixelView::Reason::CHANGED:
      PixelChanged();
      break;
   case MPix::CmdUIUpdatePixelView::Reason::MOVING:
      PixelMoved();
      break;
   case MPix::CmdUIUpdatePixelView::Reason::ACCEPT:
      PixelAccepted();
      break;
   case MPix::CmdUIUpdatePixelView::Reason::RESET:
      PixelReset();
      break;
   case MPix::CmdUIUpdatePixelView::Reason::DIED:
      PixelDied();
      break;
   case MPix::CmdUIUpdatePixelView::Reason::RESURRECT:
      PixelResurrect();
      break;
   case MPix::CmdUIUpdatePixelView::Reason::KILLED:
      PixelKilled();
      break;
   default:
      return false;
   }
   return true;
}

void MPix::PixelView::RunLockingAction( Node* model, FiniteTimeAction* action )
{
   auto unlock_functor = CallFunc::create( std::bind(&GameStateManager::Busy, &GameStateManager::getInstance(), false));
   GameStateManager::getInstance().Busy(true);
   auto s = Sequence::createWithTwoActions(action, unlock_functor);
   model->runAction(s);
}

void MPix::PixelView::PixelCreated()
{
   contents->setScale(0.01f);
   auto m_act = ScaleTo::create(0.5f, 1.0f);
   auto pause = DelayTime::create(rand()%100 / 100.0f);
   auto sq = Sequence::createWithTwoActions(pause, m_act);
   contents->runAction(sq);
}

void MPix::PixelView::PixelMoved()
{
   auto p = model->GetPos();
   auto m_act = MoveTo::create(0.08f, LogicToScreen(p) + MPIX_CELL_SIZE_HALF_P);
   RunLockingAction(contents, m_act);
}

void MPix::PixelView::PixelReset()
{
   auto sq = Sequence::create(
      Shake::create(0.5,10), 
      MoveTo::create(0.1f, LogicToScreen(model->GetPos()) + MPIX_CELL_SIZE_HALF_P),
      nullptr
      );
   RunLockingAction(contents, sq);
}

void MPix::PixelView::PixelDied()
{
   auto m_act = Spawn::create( 
      FadeOut::create(0.5f),
      ScaleTo::create(0.5f, 0.001f),
      nullptr
   );
   contents->runAction(m_act);
}

void MPix::PixelView::PixelAccepted()
{
   auto m_act = FadeOut::create(0.5f);
   contents->runAction(m_act);
}

void MPix::PixelView::PixelResurrect()
{
   auto m_act = Spawn::create( 
      FadeIn::create(0.1f),
      ScaleTo::create(0.1f, 1.0f),
      nullptr
   );
   contents->runAction(m_act);
}

void MPix::PixelView::PixelKilled()
{
   // Do nothing
}



void MPix::PixelView::PixelChanged()
{
   // Do nothing
}









