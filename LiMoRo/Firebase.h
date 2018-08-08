/**
 * Firebase.h
 * Copyright (c) 2018 takjn  All right reserved.
 */

#ifndef _FIREBASE_H_
#define _FIREBASE_H_

#include <mbed.h>
#include "https_request.h"

/* List of trusted root CA certificates
* This is the root CA for Google from GeoTrustGlobalCA and the CA for *.azurewebsites.net from Microsoft Corporation
*
* To add more root certificates, just concatenate them.
*/
const char SSL_CA_PEM[] = "-----BEGIN CERTIFICATE-----\n"
    "MIIDVDCCAjygAwIBAgIDAjRWMA0GCSqGSIb3DQEBBQUAMEIxCzAJBgNVBAYTAlVT\n"
    "MRYwFAYDVQQKEw1HZW9UcnVzdCBJbmMuMRswGQYDVQQDExJHZW9UcnVzdCBHbG9i\n"
    "YWwgQ0EwHhcNMDIwNTIxMDQwMDAwWhcNMjIwNTIxMDQwMDAwWjBCMQswCQYDVQQG\n"
    "EwJVUzEWMBQGA1UEChMNR2VvVHJ1c3QgSW5jLjEbMBkGA1UEAxMSR2VvVHJ1c3Qg\n"
    "R2xvYmFsIENBMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA2swYYzD9\n"
    "9BcjGlZ+W988bDjkcbd4kdS8odhM+KhDtgPpTSEHCIjaWC9mOSm9BXiLnTjoBbdq\n"
    "fnGk5sRgprDvgOSJKA+eJdbtg/OtppHHmMlCGDUUna2YRpIuT8rxh0PBFpVXLVDv\n"
    "iS2Aelet8u5fa9IAjbkU+BQVNdnARqN7csiRv8lVK83Qlz6cJmTM386DGXHKTubU\n"
    "1XupGc1V3sjs0l44U+VcT4wt/lAjNvxm5suOpDkZALeVAjmRCw7+OC7RHQWa9k0+\n"
    "bw8HHa8sHo9gOeL6NlMTOdReJivbPagUvTLrGAMoUgRx5aszPeE4uwc2hGKceeoW\n"
    "MPRfwCvocWvk+QIDAQABo1MwUTAPBgNVHRMBAf8EBTADAQH/MB0GA1UdDgQWBBTA\n"
    "ephojYn7qwVkDBF9qn1luMrMTjAfBgNVHSMEGDAWgBTAephojYn7qwVkDBF9qn1l\n"
    "uMrMTjANBgkqhkiG9w0BAQUFAAOCAQEANeMpauUvXVSOKVCUn5kaFOSPeCpilKIn\n"
    "Z57QzxpeR+nBsqTP3UEaBU6bS+5Kb1VSsyShNwrrZHYqLizz/Tt1kL/6cdjHPTfS\n"
    "tQWVYrmm3ok9Nns4d0iXrKYgjy6myQzCsplFAMfOEVEiIuCl6rYVSAlk6l5PdPcF\n"
    "PseKUgzbFbS9bZvlxrFUaKnjaZC2mqUPuLk/IH2uSrW4nOQdtqvmlKXBx4Ot2/Un\n"
    "hw4EbNX/3aBd7YdStysVAq45pmp06drE57xNNB6pXE0zX5IJL4hmXXeXxx12E6nV\n"
    "5fEWCRE11azbJHFwLJhWC9kXtNHjUStedejV0NxPNO3CBWaAocvmMw==\n"
    "-----END CERTIFICATE-----"
    "-----BEGIN CERTIFICATE-----\n"
    "MIIFtDCCBJygAwIBAgIQC2qzsD6xqfbEYJJqqM3+szANBgkqhkiG9w0BAQsFADBa\n"
    "MQswCQYDVQQGEwJJRTESMBAGA1UEChMJQmFsdGltb3JlMRMwEQYDVQQLEwpDeWJl\n"
    "clRydXN0MSIwIAYDVQQDExlCYWx0aW1vcmUgQ3liZXJUcnVzdCBSb290MB4XDTE2\n"
    "MDUyMDEyNTIzOFoXDTI0MDUyMDEyNTIzOFowgYsxCzAJBgNVBAYTAlVTMRMwEQYD\n"
    "VQQIEwpXYXNoaW5ndG9uMRAwDgYDVQQHEwdSZWRtb25kMR4wHAYDVQQKExVNaWNy\n"
    "b3NvZnQgQ29ycG9yYXRpb24xFTATBgNVBAsTDE1pY3Jvc29mdCBJVDEeMBwGA1UE\n"
    "AxMVTWljcm9zb2Z0IElUIFRMUyBDQSA0MIICIjANBgkqhkiG9w0BAQEFAAOCAg8A\n"
    "MIICCgKCAgEAq+XrXaNrOZ71NIgSux1SJl19CQvGeY6rtw7fGbLd7g/27vRW5Ebi\n"
    "kg/iZwvjHHGk1EFztMuZFo6/d32wrx5s7XEuwwh3Sl6Sruxa0EiB0MXpoPV6jx6N\n"
    "XtOtksDaxpE1MSC5OQTNECo8lx0AnpkYGAnPS5fkyfwA8AxanTboskDBSqyEKKo9\n"
    "Rhgrp4qs9K9LqH5JQsdiIMDmpztd65Afu4rYnJDjOrFswpTOPjJry3GzQS65xeFd\n"
    "2FkngvvhSA1+6ATx+QEnQfqUWn3FMLu2utcRm4j6AcxuS5K5+Hg8y5xomhZmiNCT\n"
    "sCqDLpcRHX6BIGHksLmbnG5TlZUixtm9dRC62XWMPD8d0Jb4M0V7ex9UM+VIl6cF\n"
    "JKLb0dyVriAqfZaJSHuSetAksd5IEfdnPLTf+Fhg9U97NGjm/awmCLbzLEPbT8QW\n"
    "0JsMcYexB2uG3Y+gsftm2tjL6fLwZeWO2BzqL7otZPFe0BtQsgyFSs87yC4qanWM\n"
    "wK5c2enAfH182pzjvUqwYAeCK31dyBCvLmKM3Jr94dm5WUiXQhrDUIELH4Mia+Sb\n"
    "vCkigv2AUVx1Xw41wt1/L3pnnz2OW4y7r530zAz7qB+dIcHz51IaXc4UV21QuEnu\n"
    "sQsn0uJpJxJuxsAmPuekKxuLUzgG+hqHOuBLf5kWTlk9WWnxcadlZRsCAwEAAaOC\n"
    "AUIwggE+MB0GA1UdDgQWBBR6e4zBz+egyhzUa/r74TPDDxqinTAfBgNVHSMEGDAW\n"
    "gBTlnVkwgkdYzKz6CFQ2hns6tQRN8DASBgNVHRMBAf8ECDAGAQH/AgEAMA4GA1Ud\n"
    "DwEB/wQEAwIBhjAnBgNVHSUEIDAeBggrBgEFBQcDAQYIKwYBBQUHAwIGCCsGAQUF\n"
    "BwMJMDQGCCsGAQUFBwEBBCgwJjAkBggrBgEFBQcwAYYYaHR0cDovL29jc3AuZGln\n"
    "aWNlcnQuY29tMDoGA1UdHwQzMDEwL6AtoCuGKWh0dHA6Ly9jcmwzLmRpZ2ljZXJ0\n"
    "LmNvbS9PbW5pcm9vdDIwMjUuY3JsMD0GA1UdIAQ2MDQwMgYEVR0gADAqMCgGCCsG\n"
    "AQUFBwIBFhxodHRwczovL3d3dy5kaWdpY2VydC5jb20vQ1BTMA0GCSqGSIb3DQEB\n"
    "CwUAA4IBAQAR/nIGOiEKN27I9SkiAmKeRQ7t+gaf77+eJDUX/jmIsrsB4Xjf0YuX\n"
    "/bd38YpyT0k66LMp13SH5LnzF2CHiJJVgr3ZfRNIfwaQOolm552W95XNYA/X4cr2\n"
    "du76mzVIoZh90pMqT4EWx6iWu9El86ZvUNoAmyqo9DUA4/0sO+3lFZt/Fg/Hjsk2\n"
    "IJTwHQG5ElBQmYHgKEIsjnj/7cae1eTK6aCqs0hPpF/kixj/EwItkBE2GGYoOiKa\n"
    "3pXxWe6fbSoXdZNQwwUS1d5ktLa829d2Wf6l1uVW4f5GXDuK+OwO++8SkJHOIBKB\n"
    "ujxS43/jQPQMQSBmhxjaMmng9tyPKPK9\n"
    "-----END CERTIFICATE-----\n";

class Firebase {
public:

    explicit Firebase(NetworkInterface *wifi, char *command_url, char *photo_url, char *message_url);
    ~Firebase();

    string get_command(void);
    int post_photo(void *body, size_t size);
    int post_message(const char *message, size_t size);

private:

    NetworkInterface *_wifi;
    char *_command_url;
    char *_photo_url;
    char *_message_url;

    string _previous_command = "";   // 前回のコマンド

    void dump_response(HttpResponse* res);
};

#endif
