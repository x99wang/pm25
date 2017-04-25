package com.controller;

import java.io.*;

import javax.servlet.http.*;

import org.json.*;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;


@Controller

@RequestMapping("/mvctest")
public class MVCtest {
	
	@RequestMapping()
	public String test()
	{
		return "test";
	}
	
	
	@RequestMapping("/do")
	public String doIns(HttpServletRequest request)throws IOException, JSONException
	{
		StringBuffer str = new StringBuffer();   
        try {  
        		//输入流
        		System.out.println("get input.....");
        		InputStream inputa = request.getInputStream();
        		System.out.println("get input secc.");  
        		if(inputa==null)
        			return "null";
        		System.out.println("put input to Buffer.");  
        		BufferedInputStream in = new BufferedInputStream(inputa);
        		System.out.println("put input secc.");  
               
                  int i;  
                  char c;  
                  while ((i=in.read())!=-1) {  
                  c=(char)i;  
                  str.append(c);  
                  }  
                 }catch (Exception ex) {  
               ex.printStackTrace();  
               }
        System.out.println(str);
        
        System.out.println("get string secc!");
        
        JSONObject obj = new JSONObject(str.toString());
        
        
        //取键值为name的value
        System.out.println(obj.get("name"));  
       // String re=obj.get("name").toString();
       // System.out.println(re);
        //根据值get实体User
        //User user= userservice.getUserByName(obj.get("name").toString());  
        return "getok";
	}
	
}
