/*
  This code is adapted from the answer to
  https://quant.stackexchange.com/q/55088/35
*/

#include <iostream>
#include <fstream> // for tests only
#include <iomanip>
#include <pcap/pcap.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

// code generated by the sbe-tool
#include "sbe/PacketHeader.h"
#include "sbe/FramingHeader.h"
#include "sbe/MessageHeader.h"
#include "sbe/Sequence_2.h"
#include "sbe/SequenceReset_1.h"
#include "sbe/SnapshotFullRefresh_Header_30.h"
#include "sbe/SnapshotFullRefresh_Orders_MBO_71.h"
#include "sbe/OpeningPrice_15.h"
#include "sbe/TheoreticalOpeningPrice_16.h"
#include "sbe/ClosingPrice_17.h"
#include "sbe/AuctionImbalance_19.h"
#include "sbe/PriceBand_20.h"
#include "sbe/PriceBand_22.h"
#include "sbe/QuantityBand_21.h"
#include "sbe/HighPrice_24.h"
#include "sbe/LowPrice_25.h"
#include "sbe/LastTradePrice_27.h"
#include "sbe/ExecutionStatistics_56.h"
#include "sbe/SecurityStatus_3.h"
#include "sbe/SecurityGroupPhase_10.h"

/*
Decoder for the Snapshot Recovery stream

Messages that are broadcasted in this stream:

Sequence
SequenceReset
SnapshotFullRefresh_Header
SnapshotFullRefresh_Orders

OpeningPrice
TheoreticalOpeningPrice
ClosingPrice
AuctionImbalance
PriceBand
QuantityBand
HighPrice
LowPrice
LastTradePrice
ExecutionStatistics

SecurityStatus
SecurityGroupPhase
*/

// use the code generated from sbe-tool to print
// the contents of the payload for each packet
void process_payload(const u_char* payload, u_int length) {
    const char* temp = reinterpret_cast<const char*>(payload);
    char* buffer = const_cast<char*>(temp);

    sbe::PacketHeader packet_header;
    packet_header.wrap(buffer, 0, sbe::PacketHeader::sbeSchemaVersion(), length);
    std::cout << "Packet Header: " << packet_header << std::endl;

    sbe::FramingHeader framing_header;
    sbe::MessageHeader message_header;
    uint64_t offset = 16;

    while (offset < length) {
        framing_header.wrap(buffer, offset, sbe::FramingHeader::sbeSchemaVersion(),
                            length);
        offset += sbe::FramingHeader::encodedLength();
        message_header.wrap(buffer, offset, sbe::MessageHeader::sbeSchemaVersion(),
                            length);
        offset += sbe::MessageHeader::encodedLength();

        std::cout << "Framing Header: " << framing_header << std::endl;
        std::cout << "Message Header: " << message_header << std::endl;

        // message body
        switch (message_header.templateId()) {
            case sbe::Sequence_2::SBE_TEMPLATE_ID: {
                sbe::Sequence_2 sequence(buffer, offset, length,
                                         sbe::Sequence_2::sbeBlockLength(),
                                         sbe::Sequence_2::sbeSchemaVersion());
                std::cout << sequence << std::endl;
                break;
            }

            case sbe::SequenceReset_1::SBE_TEMPLATE_ID: {
                sbe::SequenceReset_1 sequence_reset(
                    buffer, offset, length, sbe::SequenceReset_1::sbeBlockLength(),
                    sbe::SequenceReset_1::sbeSchemaVersion());
                std::cout << sequence_reset << std::endl;
                break;
            }

            case sbe::SnapshotFullRefresh_Header_30::SBE_TEMPLATE_ID: {
                sbe::SnapshotFullRefresh_Header_30 snapshot_full_refresh_header(
                    buffer, offset, length,
                    sbe::SnapshotFullRefresh_Header_30::sbeBlockLength(),
                    sbe::SnapshotFullRefresh_Header_30::sbeSchemaVersion());
                std::cout << snapshot_full_refresh_header << std::endl;
                break;
            }

            case sbe::SnapshotFullRefresh_Orders_MBO_71::SBE_TEMPLATE_ID: {
                sbe::SnapshotFullRefresh_Orders_MBO_71 snapshot_full_refresh_orders(
                    buffer, offset, length,
                    sbe::SnapshotFullRefresh_Orders_MBO_71::sbeBlockLength(),
                    sbe::SnapshotFullRefresh_Orders_MBO_71::sbeSchemaVersion());
                std::cout << snapshot_full_refresh_orders << std::endl;
                break;
            }

            case sbe::OpeningPrice_15::SBE_TEMPLATE_ID: {
                sbe::OpeningPrice_15 opening_price(
                    buffer, offset, length, sbe::OpeningPrice_15::sbeBlockLength(),
                    sbe::OpeningPrice_15::sbeSchemaVersion());
                std::cout << opening_price << std::endl;
                break;
            }

            case sbe::TheoreticalOpeningPrice_16::SBE_TEMPLATE_ID: {
                sbe::TheoreticalOpeningPrice_16 theoretical_opening_price(
                    buffer, offset, length,
                    sbe::TheoreticalOpeningPrice_16::sbeBlockLength(),
                    sbe::TheoreticalOpeningPrice_16::sbeSchemaVersion());
                std::cout << theoretical_opening_price << std::endl;
                break;
            }

            case sbe::ClosingPrice_17::SBE_TEMPLATE_ID: {
                sbe::ClosingPrice_17 closing_price(
                    buffer, offset, length, sbe::ClosingPrice_17::sbeBlockLength(),
                    sbe::ClosingPrice_17::sbeSchemaVersion());
                std::cout << closing_price << std::endl;
                break;
            }

            case sbe::AuctionImbalance_19::SBE_TEMPLATE_ID: {
                sbe::AuctionImbalance_19 auction_imbalance(
                    buffer, offset, length, sbe::AuctionImbalance_19::sbeBlockLength(),
                    sbe::AuctionImbalance_19::sbeSchemaVersion());
                std::cout << auction_imbalance << std::endl;
                break;
            }

            case sbe::PriceBand_20::SBE_TEMPLATE_ID: {
                sbe::PriceBand_20 price_band(buffer, offset, length,
                                             sbe::PriceBand_20::sbeBlockLength(),
                                             sbe::PriceBand_20::sbeSchemaVersion());
                std::cout << price_band << std::endl;
                break;
            }

            case sbe::PriceBand_22::SBE_TEMPLATE_ID: {
                sbe::PriceBand_22 price_band(buffer, offset, length,
                                             sbe::PriceBand_22::sbeBlockLength(),
                                             sbe::PriceBand_22::sbeSchemaVersion());
                std::cout << price_band << std::endl;
                break;
            }

            case sbe::QuantityBand_21::SBE_TEMPLATE_ID: {
                sbe::QuantityBand_21 quantity_band(
                    buffer, offset, length, sbe::QuantityBand_21::sbeBlockLength(),
                    sbe::QuantityBand_21::sbeSchemaVersion());
                std::cout << quantity_band << std::endl;
                break;
            }

            case sbe::HighPrice_24::SBE_TEMPLATE_ID: {
                sbe::HighPrice_24 high_price(buffer, offset, length,
                                             sbe::HighPrice_24::sbeBlockLength(),
                                             sbe::HighPrice_24::sbeSchemaVersion());
                std::cout << high_price << std::endl;
                break;
            }

            case sbe::LowPrice_25::SBE_TEMPLATE_ID: {
                sbe::LowPrice_25 low_price(buffer, offset, length,
                                           sbe::LowPrice_25::sbeBlockLength(),
                                           sbe::LowPrice_25::sbeSchemaVersion());
                std::cout << low_price << std::endl;
                break;
            }

            case sbe::LastTradePrice_27::SBE_TEMPLATE_ID: {
                sbe::LastTradePrice_27 last_trade_price(
                    buffer, offset, length, sbe::LastTradePrice_27::sbeBlockLength(),
                    sbe::LastTradePrice_27::sbeSchemaVersion());
                std::cout << last_trade_price << std::endl;
                break;
            }

            case sbe::ExecutionStatistics_56::SBE_TEMPLATE_ID: {
                sbe::ExecutionStatistics_56 execution_statistics(
                    buffer, offset, length, sbe::ExecutionStatistics_56::sbeBlockLength(),
                    sbe::ExecutionStatistics_56::sbeSchemaVersion());
                std::cout << execution_statistics << std::endl;
                break;
            }

            case sbe::SecurityStatus_3::SBE_TEMPLATE_ID: {
                sbe::SecurityStatus_3 security_status(
                    buffer, offset, length, sbe::SecurityStatus_3::sbeBlockLength(),
                    sbe::SecurityStatus_3::sbeSchemaVersion());
                std::cout << security_status << std::endl;
                break;
            }

            case sbe::SecurityGroupPhase_10::SBE_TEMPLATE_ID: {
                sbe::SecurityGroupPhase_10 security_group_phase(
                    buffer, offset, length, sbe::SecurityGroupPhase_10::sbeBlockLength(),
                    sbe::SecurityGroupPhase_10::sbeSchemaVersion());
                std::cout << security_group_phase << std::endl;
                break;
            }

            default: break;
        }
        offset += message_header.blockLength();

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

        std::ofstream file("output.txt");
        std::streambuf* orig_cout_stream_buf = std::cout.rdbuf();
        std::cout.rdbuf(file.rdbuf());

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
