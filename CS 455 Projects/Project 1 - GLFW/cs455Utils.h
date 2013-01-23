/**
 * Useful utilities for 455
 */

#include <stack>

#include <Eigen/Core>

#include "Matrix455.h"
#include "Vector455.h"

#define SAFE_DELETE(o)			if (o != NULL) { delete o; o = NULL; }
#define KEY_DOWN(k)				Keyboard::GetInstance().KeyIsDown(k)
#define MAT_STACK_455			std::stack<Matrix455, std::deque<Matrix455, Eigen::aligned_allocator<Matrix455> > >
	
/**
 * Unit Test snippet taken from Dr. Ken Rodham, used in CS240 at BYU.
 */
#define TEST(cond) \
do {\
	if (!(cond)) {\
		success = false; output << "Test Failed [" << __FILE__ << ", " << __LINE__ << "]" << endl;\
	}\
}while(false)
