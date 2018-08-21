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

#include "Layer.h"

using namespace std;
using namespace Crafter;

/* Table of binded layers */
map<short_word,vector<Layer::BindPair> > Layer::BindTable;
unsigned long crc32c_table[] = {
    0x00000000, 0xF26B8303, 0xE13B70F7, 0x1350F3F4,
    0xC79A971F, 0x35F1141C, 0x26A1E7E8, 0xD4CA64EB,
    0x8AD958CF, 0x78B2DBCC, 0x6BE22838, 0x9989AB3B,
    0x4D43CFD0, 0xBF284CD3, 0xAC78BF27, 0x5E133C24,
    0x105EC76F, 0xE235446C, 0xF165B798, 0x030E349B,
    0xD7C45070, 0x25AFD373, 0x36FF2087, 0xC494A384,
    0x9A879FA0, 0x68EC1CA3, 0x7BBCEF57, 0x89D76C54,
    0x5D1D08BF, 0xAF768BBC, 0xBC267848, 0x4E4DFB4B,
    0x20BD8EDE, 0xD2D60DDD, 0xC186FE29, 0x33ED7D2A,
    0xE72719C1, 0x154C9AC2, 0x061C6936, 0xF477EA35,
    0xAA64D611, 0x580F5512, 0x4B5FA6E6, 0xB93425E5,
    0x6DFE410E, 0x9F95C20D, 0x8CC531F9, 0x7EAEB2FA,
    0x30E349B1, 0xC288CAB2, 0xD1D83946, 0x23B3BA45,
    0xF779DEAE, 0x05125DAD, 0x1642AE59, 0xE4292D5A,
    0xBA3A117E, 0x4851927D, 0x5B016189, 0xA96AE28A,
    0x7DA08661, 0x8FCB0562, 0x9C9BF696, 0x6EF07595,
    0x417B1DBC, 0xB3109EBF, 0xA0406D4B, 0x522BEE48,
    0x86E18AA3, 0x748A09A0, 0x67DAFA54, 0x95B17957,
    0xCBA24573, 0x39C9C670, 0x2A993584, 0xD8F2B687,
    0x0C38D26C, 0xFE53516F, 0xED03A29B, 0x1F682198,
    0x5125DAD3, 0xA34E59D0, 0xB01EAA24, 0x42752927,
    0x96BF4DCC, 0x64D4CECF, 0x77843D3B, 0x85EFBE38,
    0xDBFC821C, 0x2997011F, 0x3AC7F2EB, 0xC8AC71E8,
    0x1C661503, 0xEE0D9600, 0xFD5D65F4, 0x0F36E6F7,
    0x61C69362, 0x93AD1061, 0x80FDE395, 0x72966096,
    0xA65C047D, 0x5437877E, 0x4767748A, 0xB50CF789,
    0xEB1FCBAD, 0x197448AE, 0x0A24BB5A, 0xF84F3859,
    0x2C855CB2, 0xDEEEDFB1, 0xCDBE2C45, 0x3FD5AF46,
    0x7198540D, 0x83F3D70E, 0x90A324FA, 0x62C8A7F9,
    0xB602C312, 0x44694011, 0x5739B3E5, 0xA55230E6,
    0xFB410CC2, 0x092A8FC1, 0x1A7A7C35, 0xE811FF36,
    0x3CDB9BDD, 0xCEB018DE, 0xDDE0EB2A, 0x2F8B6829,
    0x82F63B78, 0x709DB87B, 0x63CD4B8F, 0x91A6C88C,
    0x456CAC67, 0xB7072F64, 0xA457DC90, 0x563C5F93,
    0x082F63B7, 0xFA44E0B4, 0xE9141340, 0x1B7F9043,
    0xCFB5F4A8, 0x3DDE77AB, 0x2E8E845F, 0xDCE5075C,
    0x92A8FC17, 0x60C37F14, 0x73938CE0, 0x81F80FE3,
    0x55326B08, 0xA759E80B, 0xB4091BFF, 0x466298FC,
    0x1871A4D8, 0xEA1A27DB, 0xF94AD42F, 0x0B21572C,
    0xDFEB33C7, 0x2D80B0C4, 0x3ED04330, 0xCCBBC033,
    0xA24BB5A6, 0x502036A5, 0x4370C551, 0xB11B4652,
    0x65D122B9, 0x97BAA1BA, 0x84EA524E, 0x7681D14D,
    0x2892ED69, 0xDAF96E6A, 0xC9A99D9E, 0x3BC21E9D,
    0xEF087A76, 0x1D63F975, 0x0E330A81, 0xFC588982,
    0xB21572C9, 0x407EF1CA, 0x532E023E, 0xA145813D,
    0x758FE5D6, 0x87E466D5, 0x94B49521, 0x66DF1622,
    0x38CC2A06, 0xCAA7A905, 0xD9F75AF1, 0x2B9CD9F2,
    0xFF56BD19, 0x0D3D3E1A, 0x1E6DCDEE, 0xEC064EED,
    0xC38D26C4, 0x31E6A5C7, 0x22B65633, 0xD0DDD530,
    0x0417B1DB, 0xF67C32D8, 0xE52CC12C, 0x1747422F,
    0x49547E0B, 0xBB3FFD08, 0xA86F0EFC, 0x5A048DFF,
    0x8ECEE914, 0x7CA56A17, 0x6FF599E3, 0x9D9E1AE0,
    0xD3D3E1AB, 0x21B862A8, 0x32E8915C, 0xC083125F,
    0x144976B4, 0xE622F5B7, 0xF5720643, 0x07198540,
    0x590AB964, 0xAB613A67, 0xB831C993, 0x4A5A4A90,
    0x9E902E7B, 0x6CFBAD78, 0x7FAB5E8C, 0x8DC0DD8F,
    0xE330A81A, 0x115B2B19, 0x020BD8ED, 0xF0605BEE,
    0x24AA3F05, 0xD6C1BC06, 0xC5914FF2, 0x37FACCF1,
    0x69E9F0D5, 0x9B8273D6, 0x88D28022, 0x7AB90321,
    0xAE7367CA, 0x5C18E4C9, 0x4F48173D, 0xBD23943E,
    0xF36E6F75, 0x0105EC76, 0x12551F82, 0xE03E9C81,
    0x34F4F86A, 0xC69F7B69, 0xD5CF889D, 0x27A40B9E,
    0x79B737BA, 0x8BDCB4B9, 0x988C474D, 0x6AE7C44E,
    0xBE2DA0A5, 0x4C4623A6, 0x5F16D052, 0xAD7D5351,
    };

namespace Crafter {

	/* Create a global and unique instance of the Protocol Factory */
	Protocol Protocol::ProtoFactory ;

	/* Verbose mode flag */
	byte ShowWarnings;

}


void Crafter::Verbose(byte value) {
	Crafter::ShowWarnings = value;
}

void Crafter::Layer::Bind(const Layer& bottom_layer, short_word proto_id) {
	short_word bottom_proto = bottom_layer.GetID();
	short_word top_proto = proto_id;

	BindTable[bottom_proto].push_back(BindPair(top_proto,bottom_layer.Fields,bottom_layer.GetSize()));
}

short_word Crafter::Layer::CheckBinding() const {
	map<short_word,vector<Layer::BindPair> >::const_iterator it = BindTable.find(GetID());
	short_word next_proto = 0;

	if(it != BindTable.end()) {

		/* Allocate space for layers */
		byte* this_layer = new byte[GetSize()];
		byte* comp_layer = new byte[GetSize()];

		/* Iterate through each BindPair */
		vector<Layer::BindPair>::const_iterator it_bind;

		for(it_bind = (*it).second.begin() ; it_bind != (*it).second.end() ; it_bind++) {
			memset(this_layer,0,GetSize());
			memset(comp_layer,0,GetSize());

			/* Get the binded pair */
			BindPair bp = (*it_bind);
			/* Number of fields */
			size_t field_count = Fields.size();

			/* Sanity check */
			if(field_count != bp.Fields.size())
				return 0;

			/* OK, lets compare both layers */
			for(size_t field = 0 ; field < field_count ; field++) {
				FieldInfo* field_ptr = bp.Fields[field];
				/* Write fields */
				if(field_ptr->IsFieldSet()) {
					Fields[field]->Write(this_layer);
					field_ptr->Write(comp_layer);
				}
			}

			/* Compare both layers */
			if(!memcmp(this_layer,comp_layer,GetSize())) {
				next_proto = bp.proto_next;
				break;
			}
		}

		delete [] this_layer;
		delete [] comp_layer;
	}

	return next_proto;
}

short_word Crafter::CheckSum(const short_word *buf, int nwords) {
	unsigned long sum;

	for(sum=0; nwords>0; nwords--)
			sum += *(buf++);

	sum = (sum >> 16) + (sum &0xffff);

	sum += (sum >> 16);

	return (unsigned short)(~sum);
}

word Crafter::CRC(const unsigned char *buf, int nwords) {
	//for(int m=0;m<nwords;m++)
          //   cout << hex << (int)buf[m] <<endl;	
        //unsigned long table[0x100];
	unsigned long crc=0xFFFFFFFF;
	/*if(!*table)
    	for(size_t i = 0; i < 0x100; ++i)
	  { 
	     unsigned long k=i;
	     for(int j = 0; j < 8; ++j)
    		k = (k & 1? 0: (unsigned long)0xEDB88320L) ^ k >> 1;
             table[i]= k ^ (unsigned long)0xFF000000L;
	  }*/
  	for(size_t i = 0; i < nwords; ++i)
    	    //crc = table[(uint8_t)crc ^ buf[i]] ^ crc >> 8;
	    crc = (crc >> 8) ^ crc32c_table[(crc ^ (int)buf[i]) & 0xFF];
	crc = (~crc) & 0xFFFFFFFF;
	//cout << crc << endl;
        return (word) crc;
}

void Crafter::Layer::HexDump(ostream& str) const {

	size_t  lSize = bytes_size;

	byte *pAddressIn = new byte[lSize];

	for (size_t i = 0 ; i < size ; i++)
		pAddressIn[i] = ((byte *)raw_data)[i];

	LayerPayload.GetPayload(pAddressIn + size);

	char szBuf[100];
	long lIndent = 1;
	long lOutLen, lIndex, lIndex2, lOutLen2;
	long lRelPos;
	struct { char *pData; unsigned long lSize; } buf;
	unsigned char *pTmp,ucTmp;
	unsigned char *pAddress = (unsigned char *)pAddressIn;

   buf.pData   = (char *)pAddress;
   buf.lSize   = lSize;

   while (buf.lSize > 0)
   {
      pTmp     = (unsigned char *)buf.pData;
      lOutLen  = (int)buf.lSize;
      if (lOutLen > 16)
          lOutLen = 16;

      /* Create a 64-character formatted output line */
      sprintf(szBuf, "                              "
                     "                      "
                     "    %08lX", (long unsigned int) (pTmp-pAddress));
      lOutLen2 = lOutLen;

      for(lIndex = 1+lIndent, lIndex2 = 53-15+lIndent, lRelPos = 0;
          lOutLen2;
          lOutLen2--, lIndex += 2, lIndex2++
         )
      {
         ucTmp = *pTmp++;

         sprintf(szBuf + lIndex, "%02X ", (unsigned short)ucTmp);
         if(!isprint(ucTmp))  ucTmp = '.'; /* NonPrintable char */
         szBuf[lIndex2] = ucTmp;

         if (!(++lRelPos & 3))             /* Extra blank after 4 bytes */
         {  lIndex++; szBuf[lIndex+2] = ' '; }
      }

      if (!(lRelPos & 3)) lIndex--;

      szBuf[lIndex  ]   = ' ';
      szBuf[lIndex+1]   = ' ';

      str << szBuf << endl;

      buf.pData   += lOutLen;
      buf.lSize   -= lOutLen;
   }

   delete [] pAddressIn;
}

/* Print Payload */
void Crafter::Layer::RawString(ostream& str) const {
	/* Print raw data in hexadecimal format */
	for(size_t i = 0 ; i < size ; i++) {
		str << "\\x";
		str << std::hex << (unsigned int)((byte *)raw_data)[i];
	}

	LayerPayload.RawString(str);

	str << endl;
}

void Crafter::Layer::PrintFields(std::ostream& str) const {
	/* Print the fields */
	Fields.Print(str);
}

void Crafter::Layer::PrintPayload(std::ostream& str) const {
	str << "Payload = ";
	LayerPayload.Print(str);
}

void Crafter::Layer::Print(std::ostream& str) const {
	str << "< ";
	str << name << " (" << dec << GetSize() << " bytes) " << ":: ";

	/* Print each one of the fields */
	PrintFields(str);

	if(GetPayloadSize())
		/* Also print the payload */
		PrintPayload(str);

	str << ">" << endl;
}

/* Allocate a number of octets into the layer */
void Crafter::Layer::allocate_words(size_t nwords) {
	allocate_bytes(nwords * sizeof(word));
}

/* Allocate a number of bytes into the layer */
void Crafter::Layer::allocate_bytes(size_t nbytes) {
	/* Delete memory allocated */
	if (size)
		delete [] raw_data;

	/* Set the size */
	size = nbytes;
	/* Size in bytes of the header */
	bytes_size = nbytes;

	/* Allocate the raw data buffer */
	raw_data = new byte[nbytes];

	/* And set the buffer to zero */
	memset(raw_data, 0, size);
}

size_t Crafter::Layer::GetData(byte* data) const {
	/* Copy the data */
	if (raw_data)
		memcpy(data,raw_data,GetHeaderSize());

	/* Put Payload, if any */
	size_t npayload = LayerPayload.GetPayload(data + GetHeaderSize());

	/* Copy the data */
	if(!TopLayer)
		return GetHeaderSize() + npayload;
	else
		return GetHeaderSize() + npayload + TopLayer->GetData(data + GetHeaderSize() + npayload);

}

size_t Crafter::Layer::GetRawData(byte* data) const {
	/* Copy the data */
	if (raw_data)
		memcpy(data,raw_data,GetHeaderSize());

	/* Put Payload, if any */
	size_t npayload = LayerPayload.GetPayload(data + GetHeaderSize());

	return GetHeaderSize() + npayload;
}

size_t Crafter::Layer::PutData(const byte* data) {
	/* Set the fields from the data provided */
	Fields.ApplyAll(&FieldInfo::Read,data);

	/* Redefine the fields in function of packet values */
	ReDefineActiveFields();

	/* And write the data into the raw pointer */
	memcpy(raw_data,data,GetHeaderSize());

	return GetHeaderSize();
}

void Crafter::Layer::ParseData(ParseInfo* info) {
	/* Construct this header from the data */
	PutData(info->raw_data + info->offset);
	info->offset += GetSize();

	/* Once we have all the fields set, parse the information on this header */
	ParseLayerData(info);
	/*
	 * After this, the info structure is updated with information on what the
	 * decoder should do in the next step
	 */
}

void Crafter::Layer::ParseLayerData(ParseInfo* info) {
	/* No more layers, default */
	info->top = 1;
}

void Crafter::Layer::RedefineField(size_t nfield) {
	/* Set field as active */
	Fields.SetActive(nfield);
}

size_t Crafter::Layer::GetRemainingSize() const {
	if (!TopLayer)
		return GetSize();
	else
		return GetSize() + TopLayer->GetRemainingSize();
}

/* Payload manipulation functions */

/* Set payload */
void Crafter::Layer::SetPayload (const byte *data, int ndata) {
	LayerPayload.SetPayload(data,ndata);
	bytes_size = size + LayerPayload.GetSize();
}

/* Add more stuff to the payload */
void Crafter::Layer::AddPayload (const byte* data, int ndata) {
	LayerPayload.AddPayload(data,ndata);
	bytes_size = size + LayerPayload.GetSize();
}

/* Set payload */
void Crafter::Layer::SetPayload (const char *data) {
	LayerPayload.SetPayload(data);
	bytes_size = size  + LayerPayload.GetSize();
}

/* Add more stuff to the payload */
void Crafter::Layer::AddPayload (const char* data) {
	LayerPayload.AddPayload(data);
	bytes_size = size + LayerPayload.GetSize();
}

/* Set payload */
void Crafter::Layer::SetPayload (const Payload& data)  {
	LayerPayload.SetPayload(data);
	bytes_size = size + LayerPayload.GetSize();
}

/* Add more stuff to the payload */
void Crafter::Layer::AddPayload (const Payload& data) {
	LayerPayload.AddPayload(data);
	bytes_size = size + LayerPayload.GetSize();
}

/* Copy the data into the pointer and returns the number of bytes copied */
size_t Crafter::Layer::GetPayload(byte* dst) const {
	return LayerPayload.GetPayload(dst);
}

Crafter::Layer::Layer() : size(0), bytes_size(0), raw_data(0), BottomLayer(0), TopLayer(0) { }

Crafter::Layer::Layer(const Layer& layer) : raw_data(0) {
	/* Put size to zero */
	size = 0;
	/* Init bottom and top layer pointer */
	BottomLayer = 0;
	TopLayer = 0;

	/* Copy Header information */
	name = layer.name;
	protoID = layer.protoID;

	/* Equal size */
	if(layer.size) allocate_bytes(layer.size);

	/* Copy the fields from the other layer */
	Fields = layer.Fields;

	PutData((const byte *)layer.raw_data);

	/* Copy the payload, if any */
	size_t npayload = layer.LayerPayload.GetSize();

	byte* payload = new byte[npayload];

	layer.LayerPayload.GetPayload(payload);

	/* Finally, set the payload */
	SetPayload(payload,npayload);

	/* And delete the allocated buffer */
	delete [] payload;
}

Layer& Crafter::Layer::operator=(const Layer& right) {

	/* Sanity check */
	if (GetName() != right.GetName())
		throw runtime_error("Cannot convert " + right.GetName() + " to " + GetName());

	Clone(right);
	return *this;
}

void Crafter::Layer::Clone(const Layer& layer) {
	/* Delete memory allocated */
	delete [] raw_data;
	raw_data = 0;

	/* Put size to zero */
	size = 0;
	/* Initialize bottom and top layer pointer */
	BottomLayer = 0;
	TopLayer = 0;

	/* Copy Header information */
	name = layer.name;
	protoID = layer.protoID;

	/* Equal size */
	if(layer.size) allocate_bytes(layer.size);

	/* Copy the fields from the other layer */
	Fields = layer.Fields;

	PutData((const byte *)layer.raw_data);

	/* Copy the payload, if any */
	size_t npayload = layer.LayerPayload.GetSize();

	byte* payload = new byte[npayload];

	layer.LayerPayload.GetPayload(payload);

	/* Finally, set the payload */
	SetPayload(payload,npayload);

	/* And delete the allocated buffer */
	delete [] payload;
}

FieldInfo* Crafter::Layer::GetFieldPtr(size_t nfield) {
	return Fields[nfield];
}

byte Crafter::Layer::IsFieldSet(size_t nfield) const {
	return Fields[nfield]->IsFieldSet();
}

void Crafter::Layer::ResetFields() {
	Fields.Apply(&FieldInfo::ResetField);
}

void Crafter::Layer::ResetField(size_t nfield) {
	Fields[nfield]->ResetField();
}

byte Crafter::RNG8() {return rand()%256; }
short_word Crafter::RNG16() {return rand()%65536; }
word Crafter::RNG32() {return 2 * rand(); }

Crafter::Layer::~Layer() {
	/* Delete memory allocated */
	delete [] raw_data;
}
