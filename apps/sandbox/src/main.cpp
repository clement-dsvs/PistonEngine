//
// Created by cleme on 17/12/2024.
//

#include <iostream>

#include "PistonEngine/utils/Logger.h"

int main() {
    piston::Logger* l_logger = piston::Logger::os_GetInstance();

    std::cout << "avant" << "\n";
    l_logger->Info("test");

    l_logger->start();
    l_logger->Info("test");
    l_logger->Info("un message");
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    l_logger->stop(true);
    std::cout << "apres" "\n";
}
