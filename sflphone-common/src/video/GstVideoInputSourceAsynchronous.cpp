#include "GstVideoInputSourceAsynchronous.h"

#include <vector>
#include <string>

#include <gst/interfaces/propertyprobe.h>
#include <gst/app/gstappsink.h>

namespace sfl 
{
	
	GstVideoInputSourceAsynchronous::GstVideoInputSourceAsynchronous()
	{
	}

	GstVideoInputSourceAsynchronous::~GstVideoInputSourceAsynchronous()
	{
	}
	
	/**
	 * @Override
	 */
	std::vector<VideoDevice*> GstVideoInputSourceAsynchronous::enumerateDevices(void)
	{
		std::vector<VideoDevice*> detectedDevices;

		std::vector<GstVideoDetectedDevice*> v4l2Devices = getV4l2Devices();
		std::vector<GstVideoDetectedDevice*> dv1394Devices = getDv1394();
		std::vector<GstVideoDetectedDevice*> ximageDevices = getXimageSource();		
		std::vector<GstVideoDetectedDevice*> videoTestSourceDevices = getVideoTestSource();
		
		detectedDevices.insert(detectedDevices.end(), v4l2Devices.begin(), v4l2Devices.end());
		detectedDevices.insert(detectedDevices.end(), dv1394Devices.begin(), dv1394Devices.end());
		detectedDevices.insert(detectedDevices.end(), ximageDevices.begin(), ximageDevices.end());
		detectedDevices.insert(detectedDevices.end(), videoTestSourceDevices.begin(), videoTestSourceDevices.end());
		
		return detectedDevices;
	}
		
	void GstVideoInputSourceAsynchronous::ensurePluginAvailability(std::vector<std::string>& plugins) throw(MissingGstPluginException)
	{
		std::vector<std::string>::iterator it;
		for (it = plugins.begin(); it < plugins.end(); it++) {
			GstElement* element = NULL;
	
			element = gst_element_factory_make((*it).c_str(), ((*it) + std::string("presencetest")).c_str());
			if (element == NULL) {
				throw new MissingGstPluginException((*it) + std::string(" gstreamer pluging is missing."));
			} else {
	    		gst_object_unref(element);
			}
		}
	}
	
	std::vector<GstVideoDetectedDevice*> GstVideoInputSourceAsynchronous::getVideoTestSource() throw(MissingGstPluginException)
	{
		GstElement* element = NULL;
		element = gst_element_factory_make ("videotestsrc", "videotestsrcpresencetest");
	
		std::vector<GstVideoDetectedDevice*> detectedDevices;
		if (element != NULL) {
			detectedDevices.push_back(new GstVideoDetectedDevice(TEST, "videotestsrc", "videotestsrc"));
	    	gst_object_unref (GST_OBJECT (element));
	  	}
	  	
	  	return detectedDevices;
	}
	
	std::vector<GstVideoDetectedDevice*> GstVideoInputSourceAsynchronous::getXimageSource() throw(MissingGstPluginException)
	{
		std::vector<std::string> neededPlugins;
		neededPlugins.push_back("ximagesrc");
		neededPlugins.push_back("videoscale");
		neededPlugins.push_back("ffmpegcolorspace");
		ensurePluginAvailability(neededPlugins);

    	std::string description = "ximagesrc ! videoscale ! ffmpegcolorspace";
  		std::vector<GstVideoDetectedDevice*> detectedDevices;
  		detectedDevices.push_back(new GstVideoDetectedDevice(XIMAGE, "ximagesrc", description));
  		
  		return detectedDevices;
	}
	
	std::vector<GstVideoDetectedDevice*> GstVideoInputSourceAsynchronous::getV4l2Devices() throw(MissingGstPluginException)
	{
		std::vector<std::string> neededPlugins;
		neededPlugins.push_back("videoscale");
		neededPlugins.push_back("ffmpegcolorspace");
		
		// Will throw at that point if a plugin is missing. We might want to handle this more gracefully.
		ensurePluginAvailability(neededPlugins);
		
		// Retreive the list of devices and their details.
		std::vector<GstVideoDetectedDevice*> detectedDevices;
		GstElement* element = NULL;
		element = gst_element_factory_make ("v4l2src", "v4l2srcpresencetest");	
	  	if (element == NULL) {
			throw new MissingGstPluginException("Missing v4l2src plugin.");	  		
	  	} else {
	    	GstPropertyProbe* probe = NULL;
	    	const GParamSpec* pspec = NULL;
	    	GValueArray* array = NULL;
	
	    	probe = GST_PROPERTY_PROBE (element);
	    	pspec = gst_property_probe_get_property(probe, "device");
			array = gst_property_probe_probe_and_get_values(probe, pspec);
				
			if (array != NULL) {
			
	  			for (guint index = 0; index < array->n_values; index++) {
					GValue* device = NULL;
					gchar* name = NULL;
					gchar* descr = NULL;
	
					device = g_value_array_get_nth (array, index);
					g_object_set_property (G_OBJECT (element), "device", device);
	
					g_object_get (G_OBJECT (element), "device-name", &name, NULL);
					
					descr = g_strdup_printf ("v4l2src device=%s ! videoscale ! ffmpegcolorspace", g_value_get_string (device));
					
					// Add to vector
					GstVideoDetectedDevice * gstDevice = new GstVideoDetectedDevice(V4L2, name, descr);
					detectedDevices.push_back(gstDevice);
					
		  			g_free (name);
					g_free (descr);
	      		}
			}
	
	      g_value_array_free (array);
	      gst_element_set_state (element, GST_STATE_NULL);
	  	}
	  
		gst_object_unref (GST_OBJECT (element));
		
		return detectedDevices;
	}
	
	std::vector<GstVideoDetectedDevice*> GstVideoInputSourceAsynchronous::getDv1394() throw(MissingGstPluginException)
	{
	
		std::vector<std::string> neededPlugins;
		neededPlugins.push_back("videoscale");
		neededPlugins.push_back("decodebin");
		neededPlugins.push_back("ffmpegcolorspace");
		
		// Will throw at that point if a plugin is missing. We might want to handle this more gracefully.
		ensurePluginAvailability(neededPlugins);

		// Retreive the list of devices and their details.
		std::vector<GstVideoDetectedDevice*> detectedDevices;
		GstElement* element = NULL;			
		element = gst_element_factory_make ("dv1394src", "dv1394srcpresencetest");
	
	  	if (element == NULL) {
			throw new MissingGstPluginException("Missing dv1394src plugin.");	  		
	  	} else {
    		GstPropertyProbe* probe = NULL;	  		
			const GParamSpec* pspec = NULL;
		    GValueArray* array = NULL;
		
		    probe = GST_PROPERTY_PROBE (element);
		    pspec = gst_property_probe_get_property (probe, "guid");
		    array = gst_property_probe_probe_and_get_values (probe, pspec);
		
		    if (array != NULL) {
				for(guint index = 0; index < array->n_values; index++) {		
					GValue* guid = NULL;
					gchar* name = NULL;
					gchar* descr = NULL;
					guid = g_value_array_get_nth (array, index);
					g_object_set_property (G_OBJECT (element), "guid", guid);
		
					g_object_get (G_OBJECT (element), "device-name", &name, NULL);
					descr = g_strdup_printf ("dv1394src guid=%Ld"
						 " ! decodebin"
						 " ! videoscale"
						 " ! ffmpegcolorspace",
						 (long long int) g_value_get_uint64 (guid));
						 
					if (name != 0) {
						GstVideoDetectedDevice * gstDevice = new GstVideoDetectedDevice(DV1394, name, descr);
						detectedDevices.push_back(gstDevice);
						g_free (name);
					}
			
					g_free (descr);
		    	}
		
		      g_value_array_free (array);
		      gst_element_set_state (element, GST_STATE_NULL);
			}
		}
		 
		if (element != NULL) {
			gst_object_unref (GST_OBJECT (element));
		}
		
		return detectedDevices;
	}

}