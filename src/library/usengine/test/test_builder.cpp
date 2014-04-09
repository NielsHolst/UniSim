#include <usbase/exception.h>
#include "../builder.h"
#include "test_builder.h"

using namespace UniSim;

void TestBuilder::testSimulation() {

}

void TestBuilder::testController() {

}

void TestBuilder::testModel() {

}

void TestBuilder::testBare() {

}

void TestBuilder::testParameter() {

}

void TestBuilder::testBegin() {

}

void TestBuilder::testEnd() {

}


//TestBuilder::testSmall() {
//    builder()
//        .simulation()
//            .parameter("title", "My simulation")
//        .controller("UniSim::Steps", "steps")
//            .parameter("maxSteps", "7300")
//            .end();
//        .model("UniSim::Calendar", "calendar")
//            .parameter("initialDate", "1/1/2010")
//            .end();
//        .model("awe::Weather", "weather")
//            .end()
//        .model("awe::Rotation", "rotation")
//            .parameter("crops", "vector {maize wheat}")
//            .end()
//        .model("awe::Crop", "maize")
//            .parameter("sowingDate", "1/5/****")
//            .parameter("harvestDate", "20/9/****")
//            .parameter("maxYield", "800")
//            .parameter("maxYieldLossPct", "80")
//            .parameter("slopeYieldLossPct", "0.5")
//            .parameter("laiCalendar", "matrix { 0 0 | 200 0 | 620 0.5 | 700 0.9 | 800 1.8 | 940 5 | 1200 5 | 1808 5 | 2572 3 }")
//            .bare("treatments")
//                .model("awe::InstantMortality", "split_spraying_1")
//                    .parameter("date", "15/5/****")
//                    .parameter("mortalities", "matrix {seedling 100 | juvenile 80 | mature 10}")
//                    .end()
//                .model("awe::InstantMortality", "split_spraying_2")
//                    .parameter("date", "30/5/****")
//                    .parameter("mortalities", "matrix {seedling 100 | juvenile 80 | mature 10}")
//                    .end()
//                .end()
//            .end()
//    ;
//}

//    Bare treatments {
//        awe::InstantMortality split_spraying_1 {
//            date: 15/5/****
//            mortalities: matrix {seedling 100 | juvenile 80 | mature 10}
//        }
//        awe::InstantMortality split_spraying_2 {
//            date: 30/5/****
//            mortalities: ((seedling 100)(juvenile 80)(mature 10))
//        }
//    }
//}
