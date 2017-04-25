package com.inDB;

import org.json.JSONException;
import org.json.JSONObject;

public class CompJson 
{
	private JSONObject obj;
	
	public CompJson(){this(null);}
	public CompJson(JSONObject obj)
	{
		this.obj = obj;
		obj = doComp(obj);
	}
	public static JSONObject doComp(JSONObject obj)
	{
		//自动补全空数据
		JSONObject comObj=obj;
		
		try
		{
			if(!obj.has("id"))
				comObj.put("id", 10000);
			if(!obj.has("ch2o"))
				comObj.put("ch2o", 0);
			if(!obj.has("co"))
				comObj.put("co", 0);
			if(!obj.has("pm25"))
				comObj.put("pm25", 0);
			if(!obj.has("pm10"))
				comObj.put("pm10", 0);
			if(!obj.has("temp"))
				comObj.put("temp", 0);
			if(!obj.has("humid"))
				comObj.put("humid", 0);
			if(!obj.has("lng"))
				comObj.put("lng", 0);
			if(!obj.has("lat"))
				comObj.put("lat", 0);
			
		}
		catch(JSONException e)
		{	e.printStackTrace();	}
		
		return comObj;
	}
	
	public JSONObject getObj()
	{
		return this.obj;
	}
	

}
