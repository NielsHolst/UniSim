#include <limits>
#include <cmath>
#include "frequency_distribution.h"
#include "test_frequency_distribution.h"

#include <boost/math/distributions/normal.hpp>
  using boost::math::normal; // typedef provides default type is double.


using namespace NHolst;

void TestFrequencyDistribution::initTestCase()
{
}

void TestFrequencyDistribution::cleanupTestCase()
{
}

void TestFrequencyDistribution::testCalculations()
{
	FrequencyDistribution dist;
	double avg(50), variance(30);
	
	normal f(avg, sqrt(variance));
	for (int i=0; i < 100; ++i) {
		dist.add(i, pdf(f, i));
		
	}
	
	static double EPS = 1000.*std::numeric_limits<double>::epsilon();
		
	//std::cout << "average: " << dist.average() << " " << avg << " " << fabs(dist.average() - avg) << "\n";
    //std::cout << "variance: " << dist.variance() << " " << variance << " " << fabs(dist.variance() - variance) << "\n";
		
	QVERIFY(fabs(dist.average() - avg) < EPS);
	QVERIFY(fabs(dist.variance() - variance) < EPS);
}
 
