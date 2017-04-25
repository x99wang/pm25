package com.inDB;

import java.sql.*;
//import java.io.*;

public class DBCon 
{  
	//FileOutputStream fout = null;
	private static String url;
	private static String user;
	private static String password;  
    
	public DBCon(){}
    public DBCon(String url,String user,String pw)
    {
    	this.url=url;
    	this.user=user;
    	this.password=pw;
    }
    
    public static Connection getDataBaseConnection()
    {  
    	
    	/*try 
    	{
			fout = new FileOutputStream("update.txt");
		} catch (FileNotFoundException e1) 
		{
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}*/
    	
        String driver="com.mysql.jdbc.Driver"; 
        
        /*
        String url="jdbc:mysql://localhost:3306/AirofTKK";  
        String user="worlduser";  
        String password="worldpassword";  
         */ 
        
        //Conn to Driver  
        try
        {  
        	//Loading Driver
            Class.forName(driver);  
            //System.out.println("Driver is OK!");  
        } catch (ClassNotFoundException e)
        {  
            e.printStackTrace();
            System.out.println("Driver is fail!");
        }  
          
        //Make Connection
        Connection conn=null;  
        
        	
        	//fout.write("start connect db.....".getBytes());
        try 
        {
			conn=DriverManager.getConnection(url, user, password);
		
        //if (!conn.isClosed()) {  
        //    System.out.println("Line MYsql Success!");  } 
        
        //fout.write("have connect  ok !!!!!!!".getBytes());
        }
        catch (SQLException e) 
        {
            //fout.write("False line: ".getBytes());  
            //fout.write(e.getMessage().getBytes());
        	System.out.println("Make Connecion fail!");
        }  
        //fout.close();
        return conn;  
    }
    
    public static Connection getDataBaseConnection(String aurl,String auser,String apw)
    {  
        String driver="com.mysql.jdbc.Driver"; 
        
        //Conn to Driver  
        try
        {  
        	//Loading Driver
            Class.forName(driver);  
            //System.out.println("Driver is OK!");  
        } catch (ClassNotFoundException e)
        {  
            e.printStackTrace();
            System.out.println("Driver is fail!");
        }  
          
        //Make Connection
        Connection conn=null;  
        
        try 
        {
			conn=DriverManager.getConnection(aurl, auser, apw);
        }
        catch (SQLException e) 
        {	System.out.println("Make Connecion fail!");   }  
        
        return conn;  
    }
   
    
	
}  