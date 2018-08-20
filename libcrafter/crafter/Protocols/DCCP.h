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
#ifndef DCCP_H_
#define DCCP_H_

#include "../Layer.h"

namespace Crafter {

    class DCCP: public Layer {

        void DefineProtocol();

        Constructor GetConstructor() const {
            return DCCP::DCCPConstFunc;
        };

        static Layer* DCCPConstFunc() {
            return new DCCP;
        };

        void Craft();

        std::string MatchFilter() const;

        void ReDefineActiveFields();

        static const byte FieldSrcPort = 0;
        static const byte FieldDstPort = 1;
        static const byte FieldDataOffset = 2;
        static const byte FieldCC = 3;
        static const byte FieldCheckSum = 4;
        static const byte FieldType = 5;
        static const byte FieldReserved = 6;
        static const byte FieldSeqNumberH = 7;
        static const byte FieldSeqNumberL = 8;
        static const byte FieldSC = 9;
        

    public:

		enum { PROTO = 0x21 };

        DCCP();

       void SetSrcPort(const short_word& value) {
            SetFieldValue(FieldSrcPort,value);
        };

        void SetDstPort(const short_word& value) {
            SetFieldValue(FieldDstPort,value);
        };

    	void SetDataOffset(const byte& value) {
            SetFieldValue(FieldDataOffset,value);
        };
		
		void SetCC(const byte& value) {
            SetFieldValue(FieldCC,value);
        };

        void SetCheckSum(const short_word& value) {
            SetFieldValue(FieldCheckSum,value);
        };
        
        void SetType(const byte& value) {
            SetFieldValue(FieldType,value);
        };
        
        void SetReserved(const byte& value) {
            SetFieldValue(FieldReserved,value);
        };
        
        void SetSeqNumberH(const short_word& value) {
            SetFieldValue(FieldSeqNumberH,value);
        };
        void SetSeqNumberL(const word& value) {
            SetFieldValue(FieldSeqNumberL,value);
        };
        
        void SetSC(const word& value) {
            SetFieldValue(FieldSC,value);
        };
        

        short_word  GetSrcPort() const {
            return GetFieldValue<short_word>(FieldSrcPort);
        };

        short_word  GetDstPort() const {
            return GetFieldValue<short_word>(FieldDstPort);
        };

		byte  GetDataOffset() const {
            return GetFieldValue<byte>(FieldDataOffset);
        };

		byte  GetCC() const {
            return GetFieldValue<byte>(FieldCC);
        };

        short_word  GetCheckSum() const {
            return GetFieldValue<short_word>(FieldCheckSum);
        };
        
        byte  GetType() const {
            return GetFieldValue<byte>(FieldType);
        };
		
		byte  GetReserved() const {
            return GetFieldValue<byte>(FieldReserved);
        };

		short_word  GetSeqNumberH() const {
            return GetFieldValue<short_word>(FieldSeqNumberH);
        };

        word  GetSeqNumberL() const {
            return GetFieldValue<word>(FieldSeqNumberL);
        };
		word  GetSC() const {
            return GetFieldValue<word>(FieldSC);
        };
        

        ~DCCP() { /* Destructor */ };

    };

}

#endif /* DCCP_H_ */
