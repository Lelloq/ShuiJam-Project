#pragma once
#include <gtest/gtest.h>
#include <iostream>

namespace SJTester
{
	static int runtests()
	{
		::testing::InitGoogleTest();
		RUN_ALL_TESTS();

		std::cout << std::endl << "Press enter to continue" << std::endl;
		std::getchar();
		return 0;
	};
};