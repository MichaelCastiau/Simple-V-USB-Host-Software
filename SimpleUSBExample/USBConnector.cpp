#include "stdafx.h"
#include "USBConnector.h"
#include <iostream>

namespace usb {

	USBConnector::USBConnector(unsigned int vendorId, unsigned int productId)
	{
		//Set p/v pair so that we may connect in the next stage
		this->setProductId(productId);
		this->setVendorId(vendorId);
	}

	USBConnector::~USBConnector()
	{
		this->closeConnection();
	}
	void USBConnector::connect()
	{
		if (this->isConnected()) {
			//Device is not null, so we already have an opened connection
			//The proceed, you should close the connection first and then try again
			throw exceptions::DeviceAlreadyOpenException(
				"Error opening device, device already open", __FILE__,
				__LINE__);
		}

		//so far, the device was not yet opened
		//Initialize the usb environment
		if (libusb_init(&this->context) != 0) {
			//could not initialize libusb, severe error!
			throw runtime_error("Error: could not initialize usb environment");
		}
		//check for the device and get the handler
		this->deviceHandle = libusb_open_device_with_vid_pid(this->context,
			this->vendorId, this->productId);
		if (this->deviceHandle == NULL) {
			//No device handler (this error should normally not occur)
			//If the software found the device , but could not get the handler, this would mean that the driver 
			//of the device is not compatible with libusb
			//mind you that as of newer versions, communicating through the libusb lib requires having
			//a WinUSB driver installed on the device when using MS Windows
			stringstream stream;
			stream << "Error searching for device with vendor id " << vendorId << " and product id " << productId << ", device not connected";
			throw exceptions::DeviceNotConnectedException(stream.str(), __FILE__, __LINE__);
		}
		//get the device
		this->device = libusb_get_device(this->deviceHandle);
		//Initially, V-USB firmware only has one configuration
		int code = libusb_set_configuration(this->deviceHandle, 1);
		if (code < 0) {
			//Could not set the configuration, severe error
			this->checkErrorCode(code);
		}
		//Claim the default interface
		code = libusb_claim_interface(this->deviceHandle, 0);
		if (code < 0) {
			//Could not claim the default interface, severe error
			this->checkErrorCode(code);
		}
		//If we get here, we successfully connected with the device
		this->connectionIsOpen = true;
	}
	void USBConnector::closeConnection()
	{
		if (this->isConnected()) {
			//We will only perform operations if we got an open connection
			//Release the interface so that other software (or OS) can use the device if needed
			libusb_release_interface(this->deviceHandle, 0);
			libusb_close(this->deviceHandle);
			libusb_exit(this->context);
			//Don't forget to free the memory we self-assigned
			if (this->deviceDescriptor) {
				delete this->deviceDescriptor;
			}
		}
		this->connectionIsOpen = false;
	}
	bool USBConnector::isConnected()
	{
		return this->connectionIsOpen;
	}
	void USBConnector::sendData(uint8_t bRequest, uint8_t wValue, uint8_t wIndex, unsigned int timeout)
	{
		int numOfBytes = libusb_control_transfer(this->deviceHandle,
			LIBUSB_RECIPIENT_DEVICE | LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_ENDPOINT_OUT, bRequest, wValue,
			wIndex, this->buffer, this->lengthBuffer, timeout);
		if (numOfBytes < 0) {
			//error
			this->checkErrorCode(numOfBytes);
			//the method is wrapped in a error code, so that the application may throw an appropriate exception if something goes wrong
		}
	}
	unsigned char* USBConnector::getData(uint8_t bRequest, uint8_t wValue, uint8_t wIndex, unsigned int timeout) {
		int numOfBytes = libusb_control_transfer(this->deviceHandle,
			LIBUSB_RECIPIENT_DEVICE | LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_ENDPOINT_IN, bRequest, wValue,
			wIndex, this->buffer, this->lengthBuffer, timeout);
		if (numOfBytes < 0) {
			//error
			this->checkErrorCode(numOfBytes);
			//the method is wrapped in a error code, so that the application may throw an appropriate exception if something goes wrong
		}
		return this->buffer;
	}
	void USBConnector::setVendorId(unsigned int vendorId)
	{
		if (vendorId > 0xFFFF)	//id must be in specific range
			throw exceptions::InvalidArgumentException("Error setting vendor ID, ID must be less than 0xFFFF",
				__FILE__, __LINE__);
		this->vendorId = vendorId;
	}
	void USBConnector::setProductId(unsigned int productId)
	{
		if (productId < 0x000 || productId > 0xFFFF)	//id must be in specific range
			throw exceptions::InvalidArgumentException("Error setting product ID, ID must be less than 0xFFFF",
				__FILE__, __LINE__);
		this->productId = productId;
	}
	void USBConnector::checkErrorCode(int code) {
		switch (code) {
			//Success (no error)
		case LIBUSB_SUCCESS: {
			//do nothing
			break;
		}
							 //Input/output error.
		case LIBUSB_ERROR_IO: {
			throw exceptions::USBInputOutputException(
				"Input/ouput error on usb bus", __FILE__, __LINE__);
			break;
		}
							  //Invalid parameter.
		case LIBUSB_ERROR_INVALID_PARAM: {
			throw exceptions::USBInvalidParameterException(
				"Invalid parameter on usb bus", __FILE__, __LINE__);
			break;
		}
										 //Access denied (insufficient permissions)
		case LIBUSB_ERROR_ACCESS: {
			throw exceptions::USBAccessException(
				"Insufficient permissions to acces the usb device", __FILE__,
				__LINE__);
			break;
		}
								  //No such device (it may have been disconnected)
		case LIBUSB_ERROR_NO_DEVICE: {
			throw exceptions::DeviceNotConnectedException(
				"No such device found, it may be disconnected", __FILE__,
				__LINE__);
			break;
		}
									 //Entity not found.
		case LIBUSB_ERROR_NOT_FOUND: {
			throw exceptions::DeviceNotConnectedException(
				"No such entity found, it may be disconnected", __FILE__,
				__LINE__);
			break;
		}
									 //Resource busy.
		case LIBUSB_ERROR_BUSY: {
			throw exceptions::USBBusException("Error: the bus is busy", __FILE__,
				__LINE__);
			break;
		}
								//Operation timed out.
		case LIBUSB_ERROR_TIMEOUT: {
			throw exceptions::USBBusException("Error: usb bus timeout", __FILE__,
				__LINE__);
			break;
		}
								   //Overflow.
		case LIBUSB_ERROR_OVERFLOW: {
			throw exceptions::USBBusException("Error: usb bus overflow", __FILE__,
				__LINE__);
			break;
		}
									//Pipe error.
		case LIBUSB_ERROR_PIPE: {
			throw exceptions::USBBusException("Error: usb pipe error", __FILE__,
				__LINE__);
			break;
		}
								//System call interrupted (perhaps due to signal)
		case LIBUSB_ERROR_INTERRUPTED: {
			throw exceptions::USBBusException("Error: system call interrupted",
				__FILE__, __LINE__);
			break;
		}
									   //Insufficient memory.
		case LIBUSB_ERROR_NO_MEM: {
			throw exceptions::USBMemoryException("Error: insufficient memory",
				__FILE__, __LINE__);
			break;
		}
								  //Operation not supported or unimplemented on this platform.
		case LIBUSB_ERROR_NOT_SUPPORTED: {
			throw runtime_error(
				"Error: usb operation not supported on this platform");
			break;
		}
										 //Other error.
		case LIBUSB_ERROR_OTHER: {
			throw exceptions::USBException(
				"undefined exception thrown from the usb bus", __FILE__,
				__LINE__);
			break;
		}
		default: {
			//do nothing;
			break;
		}

		}
	}

	unsigned int USBConnector::getVendorId()
	{
		return this->vendorId;
	}

	unsigned int USBConnector::getProductId()
	{
		return this->productId;
	}

} /* namespace usb */
