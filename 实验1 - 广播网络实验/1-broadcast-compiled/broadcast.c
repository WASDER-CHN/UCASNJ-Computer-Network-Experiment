#include "base.h"
#include <stdio.h>

extern ustack_t *instance;

// the memory of ``packet'' will be free'd in handle_packet().
void broadcast_packet(iface_info_t *iface, const char *packet, int len)
{
	// TODO: broadcast packet 
	fprintf(stdout, "TODO: broadcast packet.\n");
	iface_info_t *iface_entry = NULL;
	list_for_each_entry(iface_entry, &instance->iface_list, list) {
		if (iface_entry->fd != iface->fd) {
			iface_send_packet(iface_entry, packet, len);
		}
	}
}
