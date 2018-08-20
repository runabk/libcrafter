/*
Copyright (c) 2012, Esteban Pellegrino
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ESTEBAN PELLEGRINO BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "SCTP.h"
#include "IP.h"
#include "IPv6.h"

using namespace Crafter;
using namespace std;

void SCTP::ReDefineActiveFields() {

}

void SCTP::Craft() {

	/* Bottom layer name */
	short_word bottom_layer = 0;

	/* Checksum of SCTP packet */
	word checksum;

	size_t tot_length = GetRemainingSize();


	/* Bottom layer name (look for IPs layers) */
	Layer* bottom_ptr = GetBottomLayer();

	while(bottom_ptr && (bottom_ptr->GetID() != IP::PROTO) && (bottom_ptr->GetID() != IPv6::PROTO))
        bottom_ptr = bottom_ptr->GetBottomLayer();

	if(bottom_ptr)  bottom_layer = bottom_ptr->GetID();

	if (!IsFieldSet(FieldCheckSum)) {

		/* Set the checksum to zero */
		SetCheckSum(0x0);

		if(bottom_layer == IP::PROTO) {

			size_t data_length = tot_length;
			if(data_length%2 != 0) data_length++;
			vector<byte> raw_buffer(data_length,0);

			/* Setup the rest of the SCTP datagram */
			GetData(&raw_buffer[0]);
			checksum = CRC((unsigned char *)&raw_buffer[0],raw_buffer.size());

		}

		else if(bottom_layer == IPv6::PROTO) {
			/* It's OK */

			size_t data_length = tot_length;

			if(data_length%2 != 0) data_length++;

			vector<byte> raw_buffer(data_length,0);


			/* Setup the rest of the SCTP datagram */
			GetData(&raw_buffer[40]);

			checksum = CRC((unsigned char *)&raw_buffer[0],raw_buffer.size());
		}

		else {
			PrintMessage(Crafter::PrintCodes::PrintWarning,
					     "SCTP::Craft()",
				         "Bottom Layer of SCTP packet is not IP. Cannot calculate SCTP checksum.");
			checksum = 0;
		}

		/* Set the checksum to zero */
		SetCheckSum(ntohl(checksum));
		ResetField(FieldCheckSum);
	}
}

string SCTP::MatchFilter() const {
	char src_port[6];
	char dst_port[6];
	sprintf(src_port,"%d", GetSrcPort());
	sprintf(dst_port,"%d", GetDstPort());
	std::string ret_str = "ip proto 132 and dst port " + std::string(src_port) + " and src port " + std::string(dst_port);
	return ret_str;
}
