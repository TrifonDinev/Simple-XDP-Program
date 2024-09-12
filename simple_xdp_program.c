#include <linux/if_ether.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/icmp.h>
#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

// Define unlikely to provide the compiler with branch prediction hint
#define unlikely(x) __builtin_expect(!!(x), 0)

// Convert a 16-bit integer from host byte order to network byte order (big-endian)
#define htons(x) ((__be16)___constant_swab16((x)))

// Convert a 32-bit integer from host byte order to network byte order (big-endian)
#define htonl(x) ((__be32)___constant_swab32((x)))

SEC("simple_xdp_program")
int simple_xdp_program_main(struct xdp_md *ctx)
{
  // Get data pointers
  void *data_end = (void *)(long)ctx->data_end;
  void *data = (void *)(long)ctx->data;

  // Parse Ethernet header
  struct ethhdr *eth = data;

  // Check if the ethernet header is valid
  if (unlikely(eth + 1 > (struct ethhdr *)data_end))
  return XDP_DROP;

  // Check if it's not an IPv4 packet and Allow it (XDP_PASS)
  if (unlikely(eth->h_proto != htons(ETH_P_IP)))
  return XDP_PASS;

  // Move to the IP header
  struct iphdr *iph = NULL;

  // Validate IP header length
  iph = (data + sizeof(struct ethhdr));

  if (unlikely(iph + 1 > (struct iphdr *)data_end))
  return XDP_DROP;

  // Example 1: Drop packets from a specific IPv4 address (123.45.67.89)
  // 0x7B2D4359 is in Big-Endian if you are curious...
  if (iph->saddr == htonl(0x7B2D4359))
  return XDP_DROP;

  // Handle TCP packets
  if (iph->protocol == IPPROTO_TCP) {

    // Initialize TCP header
    struct tcphdr *tcph = NULL;

    // Scan TCP header
    tcph = (data + sizeof(struct ethhdr) + (iph->ihl * 4));

    // Check TCP header
    if (unlikely(tcph + 1 > (struct tcphdr *)data_end))
    {
      return XDP_DROP;
    }

    // Validate TCP header
    if (unlikely((void *)(tcph + 1) > data_end))
    return XDP_DROP;

    // Example 2: Allow SSH traffic (TCP port 22)
    if (tcph->dest == htons(22))
    return XDP_PASS;

    // Example 3: Log debug message for FTP traffic (TCP port 21)
    // You can use cat /sys/kernel/debug/tracing/trace_pipe to view the logs
    if (tcph->dest == htons(21))
    bpf_printk("Simple TCP destination port 21 debug message");

    // Handle UDP packets
  } else if (iph->protocol == IPPROTO_UDP) {

    // Initialize UDP header
    struct udphdr *udph = NULL;

    // Scan UDP header
    udph = (data + sizeof(struct ethhdr) + (iph->ihl * 4));

    // Check UDP header
    if (unlikely(udph + 1 > (struct udphdr *)data_end))
    {
      return XDP_DROP;
    }

    // Example 4: Allow DNS traffic (UDP port 53)
    if (udph->dest == htons(53))
    return XDP_PASS;

    // Handle ICMP packets
  } else if (iph->protocol == IPPROTO_ICMP) {

    // Initialize ICMP header
    struct icmphdr *icmph = NULL;

    // Scan ICMP header
    icmph = (data + sizeof(struct ethhdr) + (iph->ihl * 4));

    // Check ICMP header
    if (unlikely(icmph + 1 > (struct icmphdr *)data_end))
    {
      return XDP_DROP;
    }

    // Example 5: Log a debug message for all ICMP packets
    // You can use cat /sys/kernel/debug/tracing/trace_pipe to view the logs
    bpf_printk("Received ICMP packet: type=%d, code=%d", icmph->type, icmph->code);
  }

  // Default action: Allow the packet if no rules are matched
  return XDP_PASS;
}

char _license[] SEC("license") = "GPL";
