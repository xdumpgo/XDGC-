#include "hwid.h"
#include <Windows.h>
#include <intrin.h>
#include <iphlpapi.h>
#include <cstdio>
#include <cstdint>
#include <string>
#include <vector>
#include <algorithm>

std::string _fingerprint;

#pragma comment (lib, "Iphlpapi.lib")

// we just need this for purposes of unique machine id. So any one or two mac's is       
// fine. 
uint16_t hashMacAddress( PIP_ADAPTER_INFO info )
{
    uint16_t hash = 0;
    for ( uint32_t i = 0; i < info->AddressLength; i++ )
    {
        hash += ( info->Address[i] << (( i & 1 ) * 8 ));
    }
    return hash;
}

void getMacHash( uint16_t& mac1, uint16_t& mac2 )
{
    IP_ADAPTER_INFO AdapterInfo[32];
    DWORD dwBufLen = sizeof( AdapterInfo );

    DWORD dwStatus = GetAdaptersInfo( AdapterInfo, &dwBufLen );
    if ( dwStatus != ERROR_SUCCESS )
        return; // no adapters.

    PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
    mac1 = hashMacAddress( pAdapterInfo );
    if ( pAdapterInfo->Next )
        mac2 = hashMacAddress( pAdapterInfo->Next );

    // sort the mac addresses. We don't want to invalidate
    // both macs if they just change order.
    if ( mac1 > mac2 )
    {
        uint16_t tmp = mac2;
        mac2 = mac1;
        mac1 = tmp;
    }
}

uint16_t getVolumeHash()
{
    DWORD serialNum = 0;

    // Determine if this volume uses an NTFS file system.
    GetVolumeInformation((LPCSTR) "c:\\", NULL, 0, &serialNum, NULL, NULL, NULL, 0 );
    uint16_t hash = (uint16_t)(( serialNum + ( serialNum >> 16 )) & 0xFFFF );

    return hash;
}

uint16_t getCpuHash()
{
    int cpuinfo[4] = { 0, 0, 0, 0 };
    __cpuid( cpuinfo, 0 );
    uint16_t hash = 0;
    uint16_t* ptr = (uint16_t*)(&cpuinfo[0]);
    for ( uint32_t i = 0; i < 8; i++ )
        hash += ptr[i];

    return hash;
}

const char* getMachineName()
{
    static char computerName[1024];
    DWORD size = 1024;
    GetComputerName( (LPSTR)computerName, &size );
    return &(computerName[0]);
}

std::string to_hex(std::string input) {
    std::vector<unsigned char> output(input.length());
    std::transform(input.begin(), input.end(), output.begin(),
                   [](char c)
                   {
                       return static_cast<unsigned char>(c);
                   });
    static const char digits[] = "0123456789abcdef";

    std::string result;

    for (int i=0; i<output.size(); i++) {
        result += output[output[i] / 16];
        result += output[output[i] % 16];
    }
    return result;
}

std::string Fingerprint() {
    if (strlen(_fingerprint.c_str()) > 0) {
        return _fingerprint;
    }

    uint16_t mac1;
    uint16_t mac2;
    getMacHash(mac1, mac2);
    auto vol_hash = getVolumeHash();
    auto name = getMachineName();
    auto cpu = getCpuHash();
    char buf [1024];
    sprintf(buf, "%u %u %u %u %s", mac1, mac2, vol_hash, cpu, name);
    _fingerprint = to_hex(std::string());
    return _fingerprint;
}