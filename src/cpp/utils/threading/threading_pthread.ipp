// Copyright 2023 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <pthread.h>
#include <string.h>
#include <stdio.h>

namespace eprosima {

void set_name_to_current_thread(
        const char* fmt,
        uint32_t arg)
{
    char thread_name[16]{};
    snprintf(thread_name, 16, fmt, arg);
    auto id = pthread_self();
    pthread_setname_np(id, thread_name);
}

void set_name_to_current_thread(
        const char* name)
{
    char thread_name[16]{};
    strncpy(thread_name, name, 15);
    thread_name[15] = '\0';
    auto id = pthread_self();
    pthread_setname_np(id, thread_name);
}

}  // namespace eprosima
