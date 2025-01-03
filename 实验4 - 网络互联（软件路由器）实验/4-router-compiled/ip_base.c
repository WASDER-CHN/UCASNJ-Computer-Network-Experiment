#include "ip.h"
#include "icmp.h"
#include "arpcache.h"
#include "rtable.h"
#include "arp.h"

#include "log.h"

#include <stdio.h>
#include <stdlib.h>

// initialize ip header 
void ip_init_hdr(struct iphdr *ip, u32 saddr, u32 daddr, u16 len, u8 proto)
{
	ip->version = 4;
	ip->ihl = 5;
	ip->tos = 0;
	ip->tot_len = htons(len);
	ip->id = rand();
	ip->frag_off = htons(IP_DF);
	ip->ttl = DEFAULT_TTL;
	ip->protocol = proto;
	ip->saddr = htonl(saddr);
	ip->daddr = htonl(daddr);
	ip->checksum = ip_checksum(ip);
}

// lookup in the routing table, to find the entry with the same and longest prefix.
// the input address is in host byte order
rt_entry_t *longest_prefix_match(u32 dst)
{
	// fprintf(stderr, "TODO: longest prefix match for the packet.\n");
	rt_entry_t *rtb = NULL;
	rt_entry_t *rt_dest = NULL;
	u32 max_mask = 0;
	list_for_each_entry(rtb, &rtable, list) {
		if ( (rtb->dest & rtb->mask) == (dst & rtb->mask) && (rtb->mask > max_mask || (rtb->mask == 0 && max_mask == 0)) ) {
			rt_dest = rtb;
			max_mask = rtb->mask;
		}
	}
	return rt_dest;
}

// send IP packet
//
// Different from forwarding packet, ip_send_packet sends packet generated by
// router itself. This function is used to send ICMP packets.
void ip_send_packet(char *packet, int len)
{
	// fprintf(stderr, "TODO: send ip packet.\n");
	struct iphdr *iphdr = packet_to_ip_hdr(packet);
	u32 dest_ip = ntohl(iphdr->daddr);
	rt_entry_t *rt_dest = longest_prefix_match(dest_ip);

	iface_send_packet(rt_dest->iface, packet, len);
}
