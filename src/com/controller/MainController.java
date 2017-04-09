package com.controller;

import com.inDB.*;

import java.io.*;

import javax.servlet.http.HttpServletRequest;

import org.json.JSONException;
import org.json.JSONObject;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;

@Controller

@RequestMapping("/pmUpdata")
public class MainController {
	
    String url="jdbc:mysql://localhost:3306/AirofTKK";
    String user="root";
    String password="wangxi29";  
     
	@RequestMapping("/Insert")
	public String doIns(HttpServletRequest request)throws IOException, JSONException
	{
		
		FileOutputStream fout = null;
		
		StringBuffer str = new StringBuffer();   
        try {

        	//输出显示
        	fout = new FileOutputStream("out.txt");
        	
			//输入流
        	
			//System.out.println("get input.....");
			InputStream inputa = request.getInputStream();
			//System.out.println("get input secc.");
			
			if(inputa==null)
				return "null";
			//存入缓存
			//System.out.println("put input to Buffer.");  
			BufferedInputStream in = new BufferedInputStream(inputa);
			//System.out.println("put input secc.");  
			
			//存入字符串
			int i;  
			char c;  
			while ((i=in.read())!=-1)
			{  
				c=(char)i;  
				str.append(c);  
			}  
              }catch(Exception ex){  ex.printStackTrace();  }
        
        fout.write(str.toString().getBytes());
        fout.write("\n\nget string secc!".getBytes());
        
        //System.out.println(str);
        
        //System.out.println("get string secc!");
        
        JSONObject obj = new JSONObject(str.toString());
        
        
        //取键值为name的value
        //System.out.println(obj.get("name"));  
        fout.write("start insert......\n".getBytes());
        // String re=obj.get("name").toString();
        // System.out.println(re);
        
        CompJson comj = new CompJson(obj);
        
        
        
        InsertValue inv = new InsertValue(comj.getObj(),url,user,password);
        
        
        if(inv.InsertSql())
        	fout.write("insert  ok !!!!!!\n".getBytes());
        else

        	fout.write("insert  false !!!!!!\n".getBytes());
        
        
        fout.close();
        
        return "getok";
	}
	
}