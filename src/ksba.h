/* ksba.h - X509 library for the Aegypten project
 *      Copyright (C) 2001 g10 Code GmbH
 *
 * This file is part of KSBA.
 *
 * KSBA is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * KSBA is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#ifndef KSBA_H
#define KSBA_H 1

#include <time.h>

#ifdef __cplusplus
extern "C" { 
#if 0
 }
#endif
#endif

typedef enum {
  KSBA_EOF = -1,
  KSBA_No_Error = 0,
  KSBA_General_Error = 1,
  KSBA_Out_Of_Core = 2,
  KSBA_Invalid_Value = 3,
  KSBA_Not_Implemented = 4,
  KSBA_Conflict = 5,
  KSBA_Read_Error = 6,
  KSBA_Write_Error = 7,
  KSBA_Invalid_Attr = 8,
  KSBA_No_Data = 9,
  KSBA_No_Value = 10,
  KSBA_Bug = 11,
  KSBA_BER_Error = 12,
  KSBA_Element_Not_Found = 13,
  KSBA_Identifier_Not_Found = 14,
  KSBA_Value_Not_Found = 15,  /* Note, that this is not the same as No Value */
  KSBA_Syntax_Error = 16,
  KSBA_Invalid_Tag = 17,
  KSBA_Invalid_Length = 18,
  KSBA_Invalid_Keyinfo = 19,
  KSBA_Unexpected_Tag = 20,
  KSBA_Not_DER_Encoded = 21,
  KSBA_Unknown_Algorithm = 22,
  KSBA_Unsupported_Algorithm = 23,
  KSBA_Object_Too_Large = 24,
  KSBA_Object_Too_Short = 25,
  KSBA_No_CMS_Object = 26,
  KSBA_Unknown_CMS_Object = 27,
  KSBA_Unsupported_CMS_Object = 28,
  KSBA_Invalid_CMS_Object = 29,
  KSBA_Unsupported_CMS_Version = 30,
  KSBA_Unsupported_Encoding = 31,
  KSBA_Missing_Value = 32,
  KSBA_Invalid_State = 33,
  KSBA_Duplicate_Value = 34,
  KSBA_Missing_Action = 35,
  KSBA_File_Error = 36,
  KSBA_Module_Not_Found = 37,
  KSBA_Encoding_Error = 38,
  KSBA_Invalid_Index = 39,
  KSBA_Invalid_OID_String = 40,
  KSBA_Invalid_Sexp = 41,
  KSBA_Unknown_Sexp = 42,
  KSBA_Invalid_Time = 43,
  KSBA_User_Error = 44,        /* may be used by callbacks */
  KSBA_Buffer_Too_Short = 45,
} KsbaError;


typedef enum {
  KSBA_CT_NONE = 0,
  KSBA_CT_DATA = 1,
  KSBA_CT_SIGNED_DATA = 2,
  KSBA_CT_ENVELOPED_DATA = 3,
  KSBA_CT_DIGESTED_DATA = 4,
  KSBA_CT_ENCRYPTED_DATA = 5,
  KSBA_CT_AUTH_DATA = 6
} KsbaContentType;


typedef enum {
  KSBA_SR_NONE = 0,     /* never seen by libgcrypt user */
  KSBA_SR_RUNNING = 1,  /* never seen by libgcrypt user */
  KSBA_SR_GOT_CONTENT = 2,
  KSBA_SR_NEED_HASH = 3,
  KSBA_SR_BEGIN_DATA = 4,
  KSBA_SR_END_DATA = 5,
  KSBA_SR_READY = 6,
  KSBA_SR_NEED_SIG = 7,
  KSBA_SR_DETACHED_DATA = 8
} KsbaStopReason;



/* X.509 certificates are represented by this object.
   ksba_cert_new() creates such an object */
struct ksba_cert_s;
typedef struct ksba_cert_s *KsbaCert;

/* CMS objects are controlled by this object.
   ksba_cms_new() creates it */
struct ksba_cms_s;
typedef struct ksba_cms_s *KsbaCMS;

/* This is a reader object vor various purposes
   see ksba_reader_new et al. */
struct ksba_reader_s;
typedef struct ksba_reader_s *KsbaReader;

/* This is a writer object vor various purposes
   see ksba_writer_new et al. */
struct ksba_writer_s;
typedef struct ksba_writer_s *KsbaWriter;

/* This is an object to store an ASN.1 parse tree as
   create by ksba_asn_parse_file() */
struct ksba_asn_tree_s;
typedef struct ksba_asn_tree_s *KsbaAsnTree;
   

/*-- cert.c --*/
KsbaCert ksba_cert_new (void);
void     ksba_cert_release (KsbaCert cert);
KsbaError ksba_cert_read_der (KsbaCert cert, KsbaReader reader);
KsbaError ksba_cert_init_from_mem (KsbaCert cert,
                                   const void *buffer, size_t length);
const unsigned char *ksba_cert_get_image (KsbaCert cert, size_t *r_length);
KsbaError ksba_cert_hash (KsbaCert cert, int what,
                          void (*hasher)(void *,
                                         const void *,
                                         size_t length), 
                          void *hasher_arg);
const char *ksba_cert_get_digest_algo (KsbaCert cert);
unsigned char *ksba_cert_get_serial (KsbaCert cert);
char *ksba_cert_get_issuer (KsbaCert cert);
time_t ksba_cert_get_validity (KsbaCert cert, int what);
char *ksba_cert_get_subject (KsbaCert cert);
char *ksba_cert_get_public_key (KsbaCert cert);
char *ksba_cert_get_sig_val (KsbaCert cert);


/*-- cms.c --*/
KsbaCMS ksba_cms_new (void);
void    ksba_cms_release (KsbaCMS cms);
KsbaError ksba_cms_set_reader_writer (KsbaCMS cms, KsbaReader r, KsbaWriter w);

KsbaError ksba_cms_parse (KsbaCMS cms, KsbaStopReason *r_stopreason);
KsbaError ksba_cms_build (KsbaCMS cms, KsbaStopReason *r_stopreason);

KsbaContentType ksba_cms_get_content_type (KsbaCMS cms, int what);
const char *ksba_cms_get_content_oid (KsbaCMS cms, int what);
KsbaError ksba_cms_get_content_enc_iv (KsbaCMS cms, unsigned char *iv,
                                       size_t maxivlen, size_t *ivlen);
const char *ksba_cms_get_digest_algo_list (KsbaCMS cms, int idx);
KsbaError ksba_cms_get_issuer_serial (KsbaCMS cms, int idx,
                                      char **r_issuer,
                                      unsigned char **r_serial);
const char *ksba_cms_get_digest_algo (KsbaCMS cms, int idx);
KsbaCert ksba_cms_get_cert (KsbaCMS cms, int idx);
KsbaError ksba_cms_get_message_digest (KsbaCMS cms, int idx,
                                       char **r_digest, size_t *r_digest_len);
KsbaError ksba_cms_get_signing_time (KsbaCMS cms, int idx, time_t *r_sigtime);
char *ksba_cms_get_sig_val (KsbaCMS cms, int idx);
char *ksba_cms_get_enc_val (KsbaCMS cms, int idx);

void
ksba_cms_set_hash_function (KsbaCMS cms,
                            void (*hash_fnc)(void *, const void *, size_t),
                            void *hash_fnc_arg);

KsbaError ksba_cms_hash_signed_attrs (KsbaCMS cms, int idx);


KsbaError ksba_cms_set_content_type (KsbaCMS cms, int what,
                                     KsbaContentType type);
KsbaError ksba_cms_add_digest_algo (KsbaCMS cms, const char *oid);
KsbaError ksba_cms_add_signer (KsbaCMS cms, KsbaCert cert);
KsbaError ksba_cms_set_message_digest (KsbaCMS cms, int idx,
                                       const char *digest,
                                       size_t digest_len);
KsbaError ksba_cms_set_signing_time (KsbaCMS cms, int idx, time_t sigtime);
KsbaError ksba_cms_set_sig_val (KsbaCMS cms, int idx, const char *sigval);




/*-- reader.c --*/
KsbaReader ksba_reader_new (void);
void       ksba_reader_release (KsbaReader r);
int        ksba_reader_error (KsbaReader r);

KsbaError ksba_reader_set_mem (KsbaReader r,
                               const void *buffer, size_t length);
KsbaError ksba_reader_set_fd (KsbaReader r, int fd);
KsbaError ksba_reader_set_file (KsbaReader r, FILE *fp);
KsbaError ksba_reader_set_cb (KsbaReader r, 
                              int (*cb)(void*,char *,size_t,size_t*),
                              void *cb_value );

KsbaError ksba_reader_read (KsbaReader r,
                            char *buffer, size_t length, size_t *nread);
KsbaError ksba_reader_unread (KsbaReader r, const void *buffer, size_t count);
unsigned long ksba_reader_tell (KsbaReader r);

/*-- writer.c --*/
KsbaWriter ksba_writer_new (void);
void       ksba_writer_release (KsbaWriter r);
int ksba_writer_error (KsbaWriter w);
unsigned long ksba_writer_tell (KsbaWriter w);
KsbaError ksba_writer_set_fd (KsbaWriter w, int fd);
KsbaError ksba_writer_set_file (KsbaWriter w, FILE *fp);
KsbaError ksba_writer_set_cb (KsbaWriter w, 
                              int (*cb)(void*,const void *,size_t),
                              void *cb_value);
KsbaError 
ksba_writer_set_filter (KsbaWriter w, 
                        KsbaError (*filter)(void*,
                                            const void *,size_t, size_t *,
                                            void *, size_t, size_t *),
                        void *filter_arg);

KsbaError ksba_writer_write (KsbaWriter w, const void *buffer, size_t length);



/*-- asn1-parse.y --*/
int ksba_asn_parse_file (const char *filename, KsbaAsnTree *result, int debug);
void ksba_asn_tree_release (KsbaAsnTree tree);

/*-- asn1-func.c --*/
void ksba_asn_tree_dump (KsbaAsnTree tree, const char *name, FILE *fp);
KsbaError ksba_asn_create_tree (const char *mod_name, KsbaAsnTree *result);

/*-- oid.c --*/
char *ksba_oid_to_str (const char *buffer, size_t length);
int ksba_oid_from_str (const char *string, char **rbuf, size_t *rlength);


/*-- util.c --*/
void ksba_set_malloc_hooks ( void *(*new_alloc_func)(size_t n),
                             void *(*new_realloc_func)(void *p, size_t n),
                             void (*new_free_func)(void*) );
void *ksba_malloc (size_t n );
void *ksba_calloc (size_t n, size_t m );
void *ksba_realloc (void *p, size_t n);
char *ksba_strdup (const char *p);
void  ksba_free ( void *a );

/*-- errors.c (generated from this file) --*/
const char *ksba_strerror (KsbaError err);

#ifdef __cplusplus
}
#endif
#endif /*KSBA_H*/



