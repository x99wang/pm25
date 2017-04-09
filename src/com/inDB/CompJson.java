package com.inDB;

import org.json.JSONException;
import org.json.JSONObject;

public class CompJson {
	private JSONObject obj;
	public CompJson(){}
	public CompJson(JSONObject obj){
		this.obj=obj;
		doComp();
	}
	private void doComp() {
		// TODO Auto-generated method stub
		try {
			if(!obj.has("id"))
				obj.put("id", 10000);
			if(!obj.has("ch2o"))
				obj.put("ch2o", 0);
			if(!obj.has("co"))
				obj.put("co", 0);
			if(!obj.has("pm25"))
				obj.put("pm25", 0);
			if(!obj.has("pm10"))
				obj.put("pm10", 0);
			if(!obj.has("temp"))
				obj.put("temp", 0);
			if(!obj.has("humid"))
				obj.put("humid", 0);
			} catch (JSONException e) {	e.printStackTrace();	}
	}
	
	public JSONObject getObj()
	{
		return this.obj;
	}
	

}
