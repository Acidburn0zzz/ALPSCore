/*****************************************************************************
 *
 * ALPS Project: Algorithms and Libraries for Physics Simulations
 *
 * ALPS Libraries
 *
 * Copyright (C) 2010 by Lukas Gamper <gamperl@gmail.com>
 *                       Matthias Troyer <troyer@comp-phys.org>
 *
 * This software is part of the ALPS libraries, published under the ALPS
 * Library License; you can use, redistribute it and/or modify it under
 * the terms of the license, either version 1 or (at your option) any later
 * version.
 * 
 * You should have received a copy of the ALPS Library License along with
 * the ALPS Libraries; see the file LICENSE.txt. If not, the license is also
 * available from http://alps.comp-phys.org/.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT 
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE 
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE, 
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
 * DEALINGS IN THE SOFTWARE.
 *
 *****************************************************************************/

#include <vector>
#include <cstring>
#include <iostream>
#include <signal.h>
#include <boost/config.hpp>



#ifndef ALPS_NG_SIGNAL_HPP
#define ALPS_NG_SIGNAL_HPP

namespace alps {
    namespace ng {
        class signal{
            public:

                signal() {
#ifndef BOOST_MSVC
                    static bool initialized;
                    if (!initialized) {
                        static struct sigaction action;
                        initialized = true;
                        memset(&action, 0, sizeof(action));
                        action.sa_handler = &signal::slot;
                        sigaction(SIGINT, &action, NULL);
                        sigaction(SIGTERM, &action, NULL);
                        sigaction(SIGXCPU, &action, NULL);
                        sigaction(SIGQUIT, &action, NULL);
                        sigaction(SIGUSR1, &action, NULL);
                        sigaction(SIGUSR2, &action, NULL);
                        sigaction(SIGSTOP, &action, NULL);
                    }
#endif
                }

                bool empty() {
                    return !signals_.size();
                }

                int top() {
                    return signals_.back();
                }

                void pop() {
                    return signals_.pop_back();
                }

                static void slot(int signal) {
                    std::cerr << "Received signal " << signal << std::endl;
                    signals_.push_back(signal);
                }

            private:

                static std::vector<int> signals_;
        };
    }
}

#endif
