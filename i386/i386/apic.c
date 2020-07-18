#include <i386/apic.h>
#include <string.h>
#include <vm/vm_kern.h>


#define MAX_CPUS 256

volatile ApicLocalUnit* lapic = NULL;

struct apic_info apic_data;

/* apic_data_init: initialize the apic_data structures to preliminary values
 *  Reserve memory to the lapic list dynamic vector
 *  Returns 0 if success, -1 if error
 */

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

/* apic_lapic_init: initialize lapic pointer to the memory common address
 *    Receives as input a pointer to the virtual memory address, previously mapped in a page
 */

void
apic_lapic_init(ApicLocalUnit* lapic_ptr)
    {
        lapic = lapic_ptr;
    }

/* apic_add_cpu: add a new lapic/cpu entry to the cpu_lapic list
 *   Receives as input the lapic's APIC ID
 */

void
apic_add_cpu(uint16_t apic_id)
{
    int numcpus = apic_data.ncpus;
    apic_data.cpu_lapic_list[numcpus] = apic_id;
    apic_data.ncpus++;
}

/* apic_add_ioapic: add a new ioapic entry to the ioapic list
 *   Receives as input an ioapic_data structure, filled with the IOAPIC entry's data
 */

void
apic_add_ioapic(struct ioapic_data ioapic)
{
    int nioapic = apic_data.nioapics;

    apic_data.ioapic_list[nioapic] = ioapic;

    apic_data.nioapics++;
}

/* apic_add_irq_override: add a new IRQ to the irq_override list
 *   Receives as input an irq_override_data structure, filled with the IRQ entry's data
 */

void
apic_add_irq_override(struct irq_override_data irq_over)
{
    int nirq = apic_data.nirqoverride;

    apic_data.irq_override_list[nirq] = irq_over;
    apic_data.nirqoverride++;
}

/* apic_get_cpu_apic_id: returns the apic_id of a cpu
 *   Receives as input the kernel ID of a CPU
 */

uint16_t
apic_get_cpu_apic_id(int kernel_id)
{
    uint16_t apic_id;

    if(kernel_id < MAX_CPUS)
        {
            apic_id = apic_data.cpu_lapic_list[kernel_id];
        }
    else
        {
            apic_id = -1;
        }

    return apic_id;
}

/* apic_get_lapic: returns a reference to the common memory address for Local APIC */

ApicLocalUnit*
apic_get_lapic(void)
{
    return lapic;
}

/* apic_get_ioapic: returns the IOAPIC identified by its kernel ID
 *   Receives as input the IOAPIC's Kernel ID
 *   Returns a ioapic_data structure with the IOAPIC's data
 */

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

/* apic_get_numcpus: returns the current number of cpus */

int
apic_get_numcpus(void)
{
    return apic_data.ncpus;
}

/* apic_get_num_ioapics: returns the current number of ioapics */

int
apic_get_num_ioapics(void)
{
    return apic_data.nioapics;
}

/* apic_refit_cpulist: adjust the size of cpu_lapic array to fit the real number of cpus
 *   instead the maximum number
 *
 * Returns 0 if success, -1 if error
 */

int apic_refit_cpulist(void)
{
    uint16_t* old_list = apic_data.cpu_lapic_list;
    uint16_t* new_list = (uint16_t*) kalloc(apic_data.ncpus*sizeof(uint16_t));
    int i = 0;
    int success = 0;


    if(new_list != NULL && old_list != NULL)
        {
            for(i = 0; i < apic_data.ncpus; i++)
                {
                    new_list[i] = old_list[i];
                }

            apic_data.cpu_lapic_list = new_list;
            kfree(old_list);
        }
    else
        {
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