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
#include <stdio.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/sysinfo.h>
#include <sys/time.h>
#include <sys/types.h>

namespace eprosima {

// Thread configuration struct.
// Defaults to nice value 0, and not changing CPU affinity.
struct ThreadConfig_t
{
    int sched_class = SCHED_OTHER;
    int sched_priority = 0;
    uint32_t cpu_mask = 0;
};

static const ThreadConfig_t thread_configuration[(unsigned int)fastdds_thread_kind_t::THREAD_KIND_NUMBER]
{
    // LOG_THREAD: one of these per process
    ThreadConfig_t{},
    // SHM_WATCHDOG_THREAD: one of these per process
    ThreadConfig_t{},
    // TIMED_EVENTS_THREAD: one of these per participant
    ThreadConfig_t{},
    // ASYNC_THREAD: One of these per flow-controller (by default one for all ASYNCHRONOWS DataWriters)
    ThreadConfig_t{},
    // UDP_RECEPTION_THREAD: one of these per transport channel (i.e. port)
    ThreadConfig_t{},
    // SHM_RECEPTION_THREAD: one of these per transport channel (i.e. port)
    ThreadConfig_t{},
    // TCP_RECEPTION_THREAD: one of these per transport channel (i.e. port)
    ThreadConfig_t{},
    // DATA_SHARING_LISTENER_THREAD: one of these per data-sharing DataReader
    ThreadConfig_t{}    
};

static int _configure_scheduler(int m_sched_class, int m_sched_priority)
{
    pthread_t self_tid = pthread_self();
    sched_param param;
    int result = 0;
    
    memset(&param, 0, sizeof(param));
    param.sched_priority = 0;
    
    //
    // Set Scheduler Class and Priority       
    //
    
    if((m_sched_class == SCHED_OTHER) ||
       (m_sched_class == SCHED_BATCH) ||
       (m_sched_class == SCHED_IDLE)) 
    {               
        //
        // BATCH and IDLE do not have explicit priority values.
        // - Requires priorty value to be zero (0).
        
        result = pthread_setschedparam(self_tid, m_sched_class, &param);

        //
        // Sched OTHER has a nice value, that we pull from the priority parameter.
        // 
        
        if(m_sched_class == SCHED_OTHER)
        {            
            result = setpriority(PRIO_PROCESS, gettid(), m_sched_priority);
        }                
    }
    else if((m_sched_class == SCHED_FIFO) ||
            (m_sched_class == SCHED_RR))
    {
        //
        // RT Policies use a different priority numberspace.
        //
        
        param.sched_priority = m_sched_priority;
        result = pthread_setschedparam(self_tid, m_sched_class, &param);
    }

    return result;
}


static int _configure_affinity(uint32_t affinity_mask)
{    
    int a;
    int result;
    int cpu_count;
    cpu_set_t  m_cpu_set;    

    result = 0;
       
    //
    // Rebuilt the cpu set from scratch...
    //
    
    CPU_ZERO(&m_cpu_set);

    //
    // If the bit is set in our mask, set it into the cpu_set
    // We only consider up to the total number of CPU's the
    // system has.
    //

    cpu_count = get_nprocs_conf();
    
    for(a = 0; a < cpu_count; a++)
    {
        if(affinity_mask & (1 << a))
        {            
            CPU_SET(a, &m_cpu_set);
            result++;
        }
    }

    if(result > 0)
    {
        result = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &m_cpu_set);
    }
    
    return result;
}

void set_current_thread_scheduling(
        fastdds_thread_kind_t /* kind */)
{
}

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
