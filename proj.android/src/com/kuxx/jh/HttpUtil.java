package com.kuxx.jh;

import java.io.InputStream;
import java.util.List;

import org.apache.http.HttpResponse;
import org.apache.http.HttpStatus;
import org.apache.http.NameValuePair;
import android.os.AsyncTask;

public class HttpUtil
{
	private String url;
	private String type;
	private String session;
	private List<NameValuePair> pairs;
	
	HttpUtil(String url0, String type0, String session0, List<NameValuePair> pairs0)
	{
		url = url0;
		type = type0;
		session = session0;
		pairs = pairs0;
	}
	
	public HttpUtil(String url)
	{
		this(url, "GET", null, null);
	}
	
	public String getSimpleString()
	{
		String ret = null;
		
	    Object []param = new Object[4];
	    param[0] = url;
	    param[1] = type;
	    param[2] = session;
	    param[3] = pairs;
	    
	    AsyncTask<Object, Object, HttpResponse> task  = new HttpReqTask().execute(param);
	    
	    HttpResponse response = null;
	    try 
	    {
	        response = (HttpResponse) task.get();
	    } 
	    catch (Exception e) 
	    {
	    	e.printStackTrace();
	        return ret;
	    }
	    if (response == null || response.getStatusLine().getStatusCode() != HttpStatus.SC_OK)
	    	return ret;
	    
	    InputStream inStream;
	    StringBuilder sb = new StringBuilder();
	    try 
	    {
			inStream =  response.getEntity().getContent();
		    int len;
		    byte[] buf = new byte[4096];
			while ((len = inStream.read(buf)) != -1)
			{
				String s = new String(buf).substring(0, len);
				sb.append(s);
			}
			ret = sb.toString();
		} 
	    catch (Exception e) 
	    {
	    	e.printStackTrace();
		} 
	    
	    return ret;
	}
}