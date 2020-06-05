#include <i386/apic.h>
#include <string.h>

#define MAX_CPUS 256

volatile ApicLocalUnit* lapic = NULL;

struct apic_info apic_data;

int
apic_data_init(void)
    {
        int success = 0;

        apic_data.cpu_lapic_list = NULL;
        apic_data.ncpus = 0;
        apic_data.nioapics = 0;
        apic_data.cpu_lapic_list = (uint16_t*) kalloc(MAX_CPUS*sizeof(uint16_t));
        apic_data.nirqoverride = 0;

        if(apic_data.cpu_lapic_list == NULL)
            {
                success = -1;
            }

        return success;
    }

void
apic_add_cpu(uint16_t apic_id)
    {
        int numcpus = apic_data.ncpus;
        apic_data.cpu_lapic_list[numcpus] = apic_id;
        apic_data.ncpus++;
    }


void
apic_lapic_init(ApicLocalUnit* lapic_ptr)
    {
        lapic = lapic_ptr;
    }

void
apic_add_ioapic(struct ioapic_data ioapic)
    {
        int nioapic = apic_data.nioapics;

        apic_data.ioapic_list[nioapic] = ioapic;

        apic_data.nioapics++;
    }


void
apic_add_irq_override(struct irq_override_data irq_over)
    {
        int nirq = apic_data.nirqoverride;

        apic_data.irq_override_list[nirq] = irq_over;
        apic_data.nirqoverride++;
    }

uint16_t
apic_get_cpu_apic_id(int kernel_id)
    {
        uint16_t apic_id;

        if(kernel_id < 256)
            {
                apic_id = apic_data.cpu_lapic_list[kernel_id];
            }
        else
            {
                apic_id = -1;
            }

        return apic_id;
    }

ApicLocalUnit*
apic_get_lapic(void)
    {
        return lapic;
    }


struct ioapic_data
apic_get_ioapic(int kernel_id)
    {
        struct ioapic_data io_apic;

        if(kernel_id < 16)
            {
                io_apic = apic_data.ioapic_list[kernel_id];
            }

        return io_apic;
    }

int
apic_get_numcpus(void)
    {
        return apic_data.ncpus;
    }

int
apic_get_num_ioapics(void)
    {
        return apic_data.nioapics;
    }

int apic_refill_cpulist(void)
    {
        uint16_t* old_list = apic_data.cpu_lapic_list;
        uint16_t* new_list = (uint16_t*) kalloc(apic_data.ncpus*sizeof(uint16_t));
        int i = 0;
        int success = 0;


        if(new_list != NULL && old_list != NULL){
            for(i = 0; i < apic_data.ncpus; i++)
            {
                new_list[i] = old_list[i];
            }

            apic_data.cpu_lapic_list = new_list;
            kfree(old_list);
        }
        else{
            success = -1;
        }

        return success;
    }

/* apic_print_info: shows the list of Local APIC and IOAPIC
 *
 * Shows each CPU and IOAPIC, with Its Kernel ID and APIC ID
 */

void apic_print_info(void)
{
    int i;
    int ncpus, nioapics;

    ncpus = apic_get_numcpus();
    nioapics = apic_get_num_ioapics();

    uint16_t lapic_id;
    uint16_t ioapic_id;

    struct ioapic_data ioapic;

    printf("CPUS\n");
    printf("-------------------------------------------------\n");
    for(i = 0; i < ncpus; i++)
        {
            lapic_id = apic_get_cpu_apic_id(i);

            printf("CPU %d - APIC ID %x\n", i, lapic_id);
        }

    printf("\nIOAPICS\n");
    printf("-------------------------------------------------\n");

    for(i = 0; i < nioapics; i++)
        {
            ioapic = apic_get_ioapic(i);
            printf("IOAPIC %d - APIC ID %x\n", i, ioapic.apic_id);
        }
}
