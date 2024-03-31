/*
  This code is adapted from the answer to
  https://quant.stackexchange.com/q/55088/35
*/

#include <iostream>
#include <iomanip>
#include <pcap/pcap.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

// code generated by the sbe-tool
#include "sbe/PacketHeader.h"
#include "sbe/MessageHeader.h"
#include "sbe/ExecutionSummary_55.h"
#include "sbe/Sequence_2.h"
#include "sbe/SecurityStatus_3.h"

// use the code generated from sbe-tool to print
// the contents of the payload for each packet
void process_payload(const u_char* payload, u_int length) {
    const char* temp = reinterpret_cast<const char*>(payload);
    char* buffer = const_cast<char*>(temp);

    sbe::PacketHeader packetHeaderDecoder;
    sbe::MessageHeader messageHeaderDecoder;

    packetHeaderDecoder.wrap(buffer, 0, sbe::PacketHeader::sbeSchemaVersion(), length);
    messageHeaderDecoder.wrap(buffer, 20, sbe::MessageHeader::sbeSchemaVersion(), length);

    // handle each templateID
    switch (messageHeaderDecoder.templateId()) {
        case 2: {
            sbe::Sequence_2 sequenceDecoder(buffer, 28, length,
                                            sbe::Sequence_2::sbeBlockLength(),
                                            sbe::Sequence_2::sbeSchemaVersion());
            std::cout << sequenceDecoder << std::endl;
            break;
        }
        case 3: {
            sbe::SecurityStatus_3 securityStatusDecoder(
                buffer, 28, length, sbe::SecurityStatus_3::sbeBlockLength(),
                sbe::SecurityStatus_3::sbeSchemaVersion());
            std::cout << securityStatusDecoder << std::endl;
            break;
        }
        // and so on ...
        default: break;
    }

    std::cout << std::endl << std::endl;
}

// get payload from an UDP packer by skipping the header
const u_char* get_payload(const u_char* packet, u_int* length) {
    const ip* ip_;
    const udphdr* udphdr_;

    u_int ip_length;
    u_int udphdr_length;

    // ethernet header starts with destination and source addresses
    const u_char* payload = packet;
    payload += 12;

    // search for IP header; assume all other Ethernet types are vlan
    while (ntohs(*reinterpret_cast<const u_short*>(payload)) != ETHERTYPE_IP) {
        payload += 4;
    }
    payload += 2;

    // IP header can vary in length
    ip_ = reinterpret_cast<const ip*>(payload);
    ip_length = ip_->ip_hl * 4;
    payload += ip_length;

    // ensure this is UDP
    if (ip_->ip_p != IPPROTO_UDP) {
        *length = 0;
        payload = nullptr;
    } else {
        // UDP header is static length
        udphdr_ = reinterpret_cast<const udphdr*>(payload);
        udphdr_length = sizeof(udphdr);
        *length = ntohs(udphdr_->uh_ulen) - udphdr_length;
        payload += udphdr_length;
    }

    return payload;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " file.pcap" << std::endl;
        return 1;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* p = pcap_open_offline(argv[1], errbuf);
    if (p == nullptr) {
        std::cerr << "Error: " << errbuf << std::endl;
        return 1;
    }

    bool done = false;

    while (!done) {
        // read a packet
        pcap_pkthdr* header;
        const u_char* packet;
        const u_char* payload;
        u_int payload_length;
        int ret = pcap_next_ex(p, &header, &packet);

        switch (ret) {
            case 1:
                // legitimate packet
                payload = get_payload(packet, &payload_length);
                process_payload(payload, payload_length);
                break;
            case 0:
                // timeout
                break;
            case -1:
                // error
                std::cerr << "Error: " << pcap_geterr(p) << std::endl;
                return 1;
            case -2:
                // end of file
                done = true;
                break;
        }
    }

    return 0;
}