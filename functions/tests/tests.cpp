#include "../src/factory.h"
#include "../src/functions.h"
#include "../src/root_finding.h"
#include <gtest/gtest.h>

TEST(Factory, ObjectCreation) {
	TFunctionFactory factory;
	
	auto f = factory.Create("const", 2.5);
	ConstFunction g{2.5};
	ASSERT_EQ(typeid(*f), typeid(g));
	
	auto f1 = factory.Create("exp", 3);
	ExpFunction g1{3};
	ASSERT_EQ((*f1)(4), g1(4));
}

TEST(Factory, ErrorsGeneration) {
	TFunctionFactory factory;
	
	EXPECT_THROW(factory.Create("something"), std::invalid_argument);
	
	EXPECT_THROW(factory.Create("something", {7, 0, 3, 15}), std::invalid_argument);
	
	EXPECT_THROW(factory.Create("something", 0.3), std::invalid_argument);
	
	auto f = factory.Create("power", 3);
	EXPECT_THROW(*f + "abc", std::logic_error);
	
	auto g = factory.Create("const", 100);
	EXPECT_THROW(*g - 10, std::logic_error);
}

TEST(Functions, Calculation) {
	TFunctionFactory factory;
	
	auto f = factory.Create("polynomial", {7, 0, 3, 15});
	EXPECT_NEAR((*f)(10), 15307, 0.00001);
	
	auto g = factory.Create("power", 8);
	ASSERT_EQ((*g)(2), 256);
	
	auto p = factory.Create("exp", 5);
	EXPECT_NEAR((*p)(20), std::exp(5 * 20), 0.00001);
	
	auto h = factory.Create("ident");
	EXPECT_NEAR((*h)(4.567), 4.567, 0.00001);
	
	auto t = factory.Create("const", 6);
	ASSERT_EQ((*t)(600), 6);
}

TEST(Functions, ArithmeticOperations) {
	TFunctionFactory factory;
	
	auto f = factory.Create("polynomial", {7, 0, 3, 15});
	auto g = factory.Create("power", 2);
	auto h = *f + *g;
	ASSERT_EQ(h(10), 15407);
	
	auto p = *g - *f;
	ASSERT_EQ(p(5), -1932);
	
	auto f1 = factory.Create("power", 3);
	auto g1 = factory.Create("exp", 5);
	
	auto t = *f1 * *g1;
	ASSERT_EQ(t(2), 8 * std::exp(5 * 2));
	
	auto k = *f1 / *g1;
	EXPECT_NEAR(k(4), 16 / std::exp(5 * 4), 0.00001);
	
	auto f2 = factory.Create("ident");
	auto sh = *f2 * *f;
	ASSERT_EQ(sh(2), 278);
}

TEST(Functions, StringRepresentation) {
	TFunctionFactory factory;
	
	auto f = factory.Create("power", 3);
	ASSERT_EQ(f->ToString(), "x^3.000000");
	
	auto g = factory.Create("exp", 4.5);
	ASSERT_EQ(g->ToString(), "e^(4.500000*x)");
	
	auto p = factory.Create("polynomial", {0, 0, 0, 2, 0, 4});
	ASSERT_EQ(p->ToString(), "2.000000*x^3 + 4.000000*x^5");
	
	auto h = factory.Create("const", 2.456);
	ASSERT_EQ(h->ToString(), "2.456000");
	
	auto t = *p + *h;
	ASSERT_EQ(t.ToString(), "(2.000000*x^3 + 4.000000*x^5 + 2.456000)");
	
	auto q = *p / *g;
	ASSERT_EQ(q.ToString(), "(2.000000*x^3 + 4.000000*x^5) / (e^(4.500000*x))");
}

TEST(Functions, Derivative) {
	TFunctionFactory factory;
	
	auto f = factory.Create("power", 4);
	ASSERT_EQ(f->GetDeriv(3), 108);
	
	auto g = factory.Create("exp", 6.7);
	EXPECT_NEAR(g->GetDeriv(4), 6.7 * std::exp(6.7 * 4), 0.00001);
	
	auto c = factory.Create("const", 2.5);
	ASSERT_EQ(c->GetDeriv(4), 0);
	
	auto id = factory.Create("ident");
	ASSERT_EQ(id->GetDeriv(45), 1);
	
	auto p = factory.Create("polynomial", {2, 3, 4});
	EXPECT_NEAR(p->GetDeriv(4.5), 3 + 4 * 2 * 4.5, 0.00001);
}

TEST(Equations, RootFinding) {
	TFunctionFactory factory;
	
	auto f = factory.Create("polynomial", {7, 0, 3, 15});
	EXPECT_NEAR(root(*f, 1000000), -0.8484, 0.0001);
	
	auto g = factory.Create("power", 2);
	auto p = factory.Create("const", 1);
	auto h = *g - *p;
	EXPECT_NEAR(root(h, 1000000), -1, 0.0001);
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
