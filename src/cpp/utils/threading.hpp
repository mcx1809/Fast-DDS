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

#ifndef UTILS__THREADING_HPP_
#define UTILS__THREADING_HPP_

namespace eprosima {

enum class fastdds_thread_kind_t : unsigned int
{
    // One of these per process
    LOG_THREAD = 0,
    SHM_WATCHDOG_THREAD,

    // One of these per participant
    TIMED_EVENTS_THREAD,

    // One of these per flow-controller (by default one for all ASYNCHRONOWS DataWriters)
    ASYNC_THREAD,

    // One of these per transport channel (i.e. port)
    UDP_RECEPTION_THREAD,
    SHM_RECEPTION_THREAD,
    TCP_RECEPTION_THREAD,

    // One of these per data-sharing DataReader
    DATA_SHARING_LISTENER_THREAD,
};

void set_current_thread_scheduling(
        fastdds_thread_kind_t kind);

void set_name_to_current_thread(
        const char* fmt,
        uint32_t arg);

void set_name_to_current_thread(
        const char* name);

} // eprosima

#endif  // UTILS__THREADING_HPP_
