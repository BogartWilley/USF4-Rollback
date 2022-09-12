#pragma once

#include <memory>

#include <windows.h>

#include "asio.hpp"
#include "Dimps__UserApp.hxx"

namespace sf4e {
    struct UserApp : Dimps::UserApp
    {
        static std::unique_ptr<asio::io_context> io_context;

        static void Install();
        static void Steam_PostUpdate();
    };
}