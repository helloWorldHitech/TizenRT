/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

/**
 * @file
 * Network buffer management
 *
 */

#include <net/lwip/opt.h>

#if LWIP_NETCONN				/* don't build if not configured for use in lwipopts.h */

#include <net/lwip/netbuf.h>
#include <net/lwip/memp.h>

#include <string.h>

/**
 * Create (allocate) and initialize a new netbuf.
 * The netbuf doesn't yet contain a packet buffer!
 *
 * @return a pointer to a new netbuf
 *         NULL on lack of memory
 */
struct
netbuf *netbuf_new(void)
{
	struct netbuf *buf;

	buf = (struct netbuf *)memp_malloc(MEMP_NETBUF);
	if (buf != NULL) {
		memset(buf, 0, sizeof(struct netbuf));
	}
	return buf;
}

/**
 * Deallocate a netbuf allocated by netbuf_new().
 *
 * @param buf pointer to a netbuf allocated by netbuf_new()
 */
void netbuf_delete(struct netbuf *buf)
{
	if (buf != NULL) {
		if (buf->p != NULL) {
			pbuf_free(buf->p);
			buf->p = buf->ptr = NULL;
		}
		memp_free(MEMP_NETBUF, buf);
	}
}

/**
 * Allocate memory for a packet buffer for a given netbuf.
 *
 * @param buf the netbuf for which to allocate a packet buffer
 * @param size the size of the packet buffer to allocate
 * @return pointer to the allocated memory
 *         NULL if no memory could be allocated
 */
void *netbuf_alloc(struct netbuf *buf, u16_t size)
{
	LWIP_ERROR("netbuf_alloc: invalid buf", (buf != NULL), return NULL;);

	/* Deallocate any previously allocated memory. */
	if (buf->p != NULL) {
		pbuf_free(buf->p);
	}
	buf->p = pbuf_alloc(PBUF_TRANSPORT, size, PBUF_RAM);
	if (buf->p == NULL) {
		return NULL;
	}
	LWIP_ASSERT("check that first pbuf can hold size", (buf->p->len >= size));
	buf->ptr = buf->p;
	return buf->p->payload;
}

/**
 * Free the packet buffer included in a netbuf
 *
 * @param buf pointer to the netbuf which contains the packet buffer to free
 */
void netbuf_free(struct netbuf *buf)
{
	LWIP_ERROR("netbuf_free: invalid buf", (buf != NULL), return;);
	if (buf->p != NULL) {
		pbuf_free(buf->p);
	}
	buf->p = buf->ptr = NULL;
}

/**
 * Let a netbuf reference existing (non-volatile) data.
 *
 * @param buf netbuf which should reference the data
 * @param dataptr pointer to the data to reference
 * @param size size of the data
 * @return ERR_OK if data is referenced
 *         ERR_MEM if data couldn't be referenced due to lack of memory
 */
err_t netbuf_ref(struct netbuf *buf, const void *dataptr, u16_t size)
{
	LWIP_ERROR("netbuf_ref: invalid buf", (buf != NULL), return ERR_ARG;);
	if (buf->p != NULL) {
		pbuf_free(buf->p);
	}
	buf->p = pbuf_alloc(PBUF_TRANSPORT, 0, PBUF_REF);
	if (buf->p == NULL) {
		buf->ptr = NULL;
		return ERR_MEM;
	}
	((struct pbuf_rom *)buf->p)->payload = dataptr;
	buf->p->len = buf->p->tot_len = size;
	buf->ptr = buf->p;
	return ERR_OK;
}

/**
 * Chain one netbuf to another (@see pbuf_chain)
 *
 * @param head the first netbuf
 * @param tail netbuf to chain after head, freed by this function, may not be reference after returning
 */
void netbuf_chain(struct netbuf *head, struct netbuf *tail)
{
	LWIP_ERROR("netbuf_chain: invalid head", (head != NULL), return;);
	LWIP_ERROR("netbuf_chain: invalid tail", (tail != NULL), return;);
	pbuf_cat(head->p, tail->p);
	head->ptr = head->p;
	memp_free(MEMP_NETBUF, tail);
}

/**
 * Get the data pointer and length of the data inside a netbuf.
 *
 * @param buf netbuf to get the data from
 * @param dataptr pointer to a void pointer where to store the data pointer
 * @param len pointer to an u16_t where the length of the data is stored
 * @return ERR_OK if the information was retreived,
 *         ERR_BUF on error.
 */
err_t netbuf_data(struct netbuf *buf, void **dataptr, u16_t *len)
{
	LWIP_ERROR("netbuf_data: invalid buf", (buf != NULL), return ERR_ARG;);
	LWIP_ERROR("netbuf_data: invalid dataptr", (dataptr != NULL), return ERR_ARG;);
	LWIP_ERROR("netbuf_data: invalid len", (len != NULL), return ERR_ARG;);

	if (buf->ptr == NULL) {
		return ERR_BUF;
	}
	*dataptr = buf->ptr->payload;
	*len = buf->ptr->len;
	return ERR_OK;
}

/**
 * Move the current data pointer of a packet buffer contained in a netbuf
 * to the next part.
 * The packet buffer itself is not modified.
 *
 * @param buf the netbuf to modify
 * @return -1 if there is no next part
 *         1  if moved to the next part but now there is no next part
 *         0  if moved to the next part and there are still more parts
 */
s8_t netbuf_next(struct netbuf *buf)
{
	LWIP_ERROR("netbuf_next: invalid buf", (buf != NULL), return -1;);
	if (buf->ptr->next == NULL) {
		return -1;
	}
	buf->ptr = buf->ptr->next;
	if (buf->ptr->next == NULL) {
		return 1;
	}
	return 0;
}

/**
 * Move the current data pointer of a packet buffer contained in a netbuf
 * to the beginning of the packet.
 * The packet buffer itself is not modified.
 *
 * @param buf the netbuf to modify
 */
void netbuf_first(struct netbuf *buf)
{
	LWIP_ERROR("netbuf_first: invalid buf", (buf != NULL), return;);
	buf->ptr = buf->p;
}

#endif							/* LWIP_NETCONN */
