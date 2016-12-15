#pragma once

#include "libusb.h"
#include "DeviceAlreadyOpenException.hpp"
#include "DeviceNotConnectedException.hpp"
#include "USBAccessException.hpp"
#include "USBBusException.hpp"
#include "USBException.hpp"
#include "USBInputOutputException.hpp"
#include "USBInvalidParameterException.hpp"
#include "USBMemoryException.hpp"
#include "USBMemoryException.hpp"
#include "IllegalArgumentException.hpp"
#include "USBTimeoutException.hpp"

namespace usb {
	class USBConnector
	{
	private:
		/// Max length of the buffer for in/out communication
		/// Here the max size is 64 bytes ( 1 char = 8 bits/1 byte)
		static const unsigned int lengthBuffer = 64;
		/// The vendor Id of the device to wich the connector is connected
		unsigned int vendorId = 0;
		/// The product Id of the device to which the connector is connected
		unsigned int productId = 0;
		/// The context gotten from the libusb library, this pointer will be freed by the lib, don't do it manually
		libusb_context* context = nullptr;
		/// The device pointer gotten from the libusb library, this pointer will be freed by the lib, don't do it manually
		libusb_device* device = nullptr;
		/// De device handler. This will be our starting point for most operaions. This pointer will be freed by the lib, don't do it manually
		libusb_device_handle* deviceHandle = nullptr;
		/// Device descriptor. This pointer is assigned manually, don't forget to delete it
		libusb_device_descriptor* deviceDescriptor = nullptr;
		/// Buffer for in/out communication
		unsigned char buffer[lengthBuffer];
		/// Holds if the connection is currently active or not
		bool connectionIsOpen = false;
	public:
		/// Default constructor
		/// @param vendorId The vendor Id in hexadecimal value
		/// @param productId The product Id in hecamdecimal value
		USBConnector(unsigned int vendorId, unsigned int productId);
		~USBConnector();
		/// Initializes the connection to the USB device
		/// Call this method before performing any operations on this connector/usb device
		void connect();
		/// Closes the connection with the USB device
		void closeConnection();
		/// Checks if the connector is currently connected with the USB device
		/// @return true if connector is connected, false otherwise
		bool isConnected();
		/// Sends a custom control request to the device. You may send arbitrary data with the request
		/// @param bRequest The request type
		/// @param wValue Arbitrary data that you may send with the request
		/// @param wIndex Arbitrary data that you may send with the request
		void sendData(uint8_t bRequest, uint8_t wValue, uint8_t wIndex, unsigned int timeout = 0);
		/// Gets data from the device and stores it in a buffer
		/// @param bRequest The request type
		/// @param wValue Arbitrary data that you may send with the request
		/// @param wIndex Arbitrary data that you may send with the request
		/// @returns unsigned char* a pointer to a byte array (the data returned by the device)
		unsigned char* getData(uint8_t bRequest, uint8_t wValue, uint8_t wIndex, unsigned int timeout = 0);
		/// Checks the error code passed as parameter and raises an exception if necessary
		/// Used to control the flow of the program in a C++/object oriented way
		void checkErrorCode(int code);

		/*
		Getters and setters
		*/
		unsigned int getVendorId();
		unsigned int getProductId();
	private:
		void setVendorId(unsigned int vendorId);
		void setProductId(unsigned int productId);
	};

}