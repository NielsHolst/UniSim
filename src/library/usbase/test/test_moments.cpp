#include <boost/math/distributions/normal.hpp>
#include <usbase/test_num.h>
#include <usbase/moments.h>
#include "test_moments.h"
using boost::math::normal; // typedef provides double as default type

void TestMoments::testCalculations()
{
    UniSim::Moments moments;
	double avg(50), variance(30);
	
	normal f(avg, sqrt(variance));
	for (int i=0; i < 100; ++i) {
        moments.append(i, pdf(f, i));
		
	}
	
    QVERIFY(TestNum::eq(moments.average(), avg));
    QVERIFY(TestNum::eq(moments.variance(), variance));
}
 
