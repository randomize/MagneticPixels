#include "Solution.h"

using namespace MPix;

//====---------------------------------------------======//

MPix::Solution::Solution()
{


}

shared_ptr<Solution> MPix::Solution::Dublicate()
{
   auto fab = new Solution(*this);
   return shared_ptr<Solution>(fab);
}
