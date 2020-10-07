// SPDX-License-Identifier: GPL-3.0
#include <linux/kernel.h>  // printk(), pr_*()
#include <linux/module.h>  // THIS_MODULE, MODULE_VERSION, ...
#include <linux/init.h>    // module_{init,exit}()
#include <linux/smp.h>     // get_cpu(), put_cpu()
#include <linux/cpufreq.h> // cpufreq_get()
#include <linux/cpumask.h> // cpumask_{first,next}(), cpu_online_mask

#ifdef pr_fmt
#undef pr_fmt
#endif
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

static int __init modinit(void)
{
        unsigned cpu = cpumask_first(cpu_online_mask);

        while (cpu < nr_cpu_ids) {
                pr_info("CPU: %u, freq: %u kHz\n", cpu, cpufreq_get(cpu));
                cpu = cpumask_next(cpu, cpu_online_mask);
        }

        return 0;
}

static void __exit modexit(void)
{
        // Empty function only to be able to unload the module.
        return;
}

module_init(modinit);
module_exit(modexit);
MODULE_VERSION("0.1");
MODULE_DESCRIPTION("Get CPU frequency for currently online CPUs.");
MODULE_AUTHOR("Marco Bonelli");
MODULE_LICENSE("GPL");