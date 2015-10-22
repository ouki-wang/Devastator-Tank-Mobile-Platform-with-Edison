/*
 * TwoWire.h - TWI/I2C library for Linux Userspace
 * Copyright (c) 2013 Parav https://github.com/meanbot.
 * All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

extern "C" {
#include <string.h>
#include <i2c.h>
}

#include "IIC1.h"

#include <Mux.h>
#include <trace.h>
#define MY_TRACE_PREFIX "Wire"

DFTwoWire::DFTwoWire(void(*_beginCb)(void)) : rxBufferIndex(0), rxBufferLength(0), 
					  txAddress(0), txBufferLength(0), 
					  srvBufferIndex(0), srvBufferLength(0), 
					  onBeginCallback(_beginCb), 
					  adapter_nr(-1), i2c_fd(-1),
					  i2c_transfer(0)
{
	// Empty
}

void DFTwoWire::begin(void)
{
	muxSelectI2c(0);
	if (onBeginCallback)
		onBeginCallback();
	if(adapter_nr < 0){
		if ((adapter_nr = i2c_getadapter(I2C2)) < 0) {
			return;	
		}
	}
	if(i2c_fd < 0){
		if ((i2c_fd = i2c_openadapter(1)) < 0) {
			return;	
		}
	}
}

void DFTwoWire::begin(uint8_t address)
{
	if (onBeginCallback)
		onBeginCallback();
}

void DFTwoWire::begin(int address)
{
	begin((uint8_t) address);
}

uint8_t DFTwoWire::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop)
{
	if (quantity > BUFFER_LENGTH)
		quantity = BUFFER_LENGTH;

	/* Set slave address via ioctl  */ 
	i2c_setslave(i2c_fd, address);

	if(i2c_transfer) {
	/* Need to perform a combined read/write operation
	 */	
		i2c_transfer = 0;
		if (sendStop == false)
			return 0;

		i2c_add_to_buf(address, 1, rxBuffer, quantity);
		if (!i2c_readwrite(i2c_fd)) {
			rxBufferIndex = 0;
			rxBufferLength = quantity;
			i2c_transfer = 0;
			return quantity;
		} else
			return 0;
	}

	if (i2c_readbytes(i2c_fd, rxBuffer, quantity) < 0)
		return 0;
	// set rx buffer iterator vars
	rxBufferIndex = 0;
	rxBufferLength = quantity;

	return quantity;
}

uint8_t DFTwoWire::requestFrom(uint8_t address, uint8_t quantity)
{
	return requestFrom((uint8_t) address, (uint8_t) quantity, (uint8_t) true);
}

uint8_t DFTwoWire::requestFrom(int address, int quantity)
{
	return requestFrom((uint8_t) address, (uint8_t) quantity, (uint8_t) true);
}

uint8_t DFTwoWire::requestFrom(int address, int quantity, int sendStop)
{
	return requestFrom((uint8_t) address, (uint8_t) quantity, (uint8_t) sendStop);
}

void DFTwoWire::beginTransmission(uint8_t address)
{
	if (i2c_fd < 0 || adapter_nr < 0)
		return;
	/* set slave address via ioctl in case we need to perform terminating 
	 * write operation
	 */
	i2c_setslave(i2c_fd, address);
	// save address of target and empty buffer
	txAddress = address;
	txBufferLength = 0;
}

void DFTwoWire::beginTransmission(int address)
{
	beginTransmission((uint8_t) address);
}

//
//	Originally, 'endTransmission' was an f(void) function.
//	It has been modified to take one parameter indicating
//	whether or not a STOP should be performed on the bus.
//	Calling endTransmission(false) allows a sketch to
//	perform a repeated start.
//
//	WARNING: Nothing in the library keeps track of whether
//	the bus tenure has been properly ended with a STOP. It
//	is very possible to leave the bus in a hung state if
//	no call to endTransmission(true) is made. Some I2C
//	devices will behave oddly if they do not see a STOP.
//
uint8_t DFTwoWire::endTransmission(uint8_t sendStop)
{
	int err;
	if (sendStop == true) {	

		// transmit buffer (blocking)
		if (txBufferLength > 1)
			err = i2c_writebytes(i2c_fd, txBuffer, txBufferLength);
		else if (txBufferLength == 1)
			err = i2c_writebyte(i2c_fd, *txBuffer);
		else
		/* FIXME: A zero byte transmit is typically used to check for an
		 * ACK from the slave device. I'm not sure if this is the
		 * correct way to do this.
		 */ 
			err = i2c_readbyte(i2c_fd);
		// empty buffer
		txBufferLength = 0;
		if (err < 0)
			return 2;
		return 0;
	} else {
	/* sendStop = false
	 * pretend we have held the bus while
	 * actually waiting for the next operation
	 */
		i2c_add_to_buf(txAddress, 0, txBuffer, txBufferLength);		
		i2c_transfer = 1;
		return 0;
	}
}

//	This provides backwards compatibility with the original
//	definition, and expected behaviour, of endTransmission
//
uint8_t DFTwoWire::endTransmission(void)
{
	return endTransmission(true);
}

size_t DFTwoWire::write(uint8_t data)
{
	if (txBufferLength >= BUFFER_LENGTH)
		return 0;
	txBuffer[txBufferLength++] = data;
	return 1;
}

size_t DFTwoWire::write(const uint8_t *data, size_t quantity)
{
	for (size_t i = 0; i < quantity; ++i) {
		if (txBufferLength >= BUFFER_LENGTH)
			return i;
		txBuffer[txBufferLength++] = data[i];
	}
	return quantity;
}

int DFTwoWire::available(void)
{
	return rxBufferLength - rxBufferIndex;
}

int DFTwoWire::read(void)
{
	if (rxBufferIndex < rxBufferLength)
		return rxBuffer[rxBufferIndex++];
	return -1;
}

int DFTwoWire::peek(void)
{
	if (rxBufferIndex < rxBufferLength)
		return rxBuffer[rxBufferIndex];
	return -1;
}

void DFTwoWire::flush(void)
{
	// Do nothing, use endTransmission(..) to force
	// data transfer.
}

void DFTwoWire::onReceive(void(*function)(int))
{
	onReceiveCallback = function;
}

void DFTwoWire::onRequest(void(*function)(void)) {
	onRequestCallback = function;
}

void DFTwoWire::onService(void)
{
}

#if WIRE_INTERFACES_COUNT > 0
static void Wire_Init(void)
{
}

DFTwoWire DFWire = DFTwoWire(Wire_Init);
/*
void WIRE_ISR_HANDLER(void) {
	DFWire.onService();
}*/
#endif

#if WIRE_INTERFACES_COUNT > 1
static void Wire1_Init(void)
{
	if ((adapter_nr = i2c_getadapter(I2C1)) < 0) {
		return;	
	}
	if ((i2c_fd = i2c_openadapter) < 0) {
		return;	
	}
}

DFTwoWire DFWire1 = DFTwoWire(Wire1_Init);
/*
void WIRE1_ISR_HANDLER(void) {
	DFWire1.onService();
}*/
#endif
