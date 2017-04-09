package com.inDB;

import java.sql.*;
import java.io.*;

public class upData 
{  
	
	static FileOutputStream fout = null;
	
	private static String url;
	private static String user;
	private static String password;  
    public upData(){}
    public upData(String url,String user,String pw)
    {
    	this.url=url;
    	this.user=user;
    	this.password=pw;
    }
    public static Connection getDataBaseConnection() throws IOException{  
    	
    	try {
			fout = new FileOutputStream("update.txt");
		} catch (FileNotFoundException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
    	
        String driver="com.mysql.jdbc.Driver";  
        /*
        String url="jdbc:mysql://localhost:3306/AirofTKK";  
        String user="worlduser";  
        String password="worldpassword";  
         */ 
        //Go Driver  
        try {  
            Class.forName(driver);  
            //System.out.println("Driver is OK!");  
        } catch (ClassNotFoundException e) {  
            e.printStackTrace();  
        }  
          
        //Go Database 
        Connection conn=null;  
        try {  
        	
        	fout.write("start connect db.....".getBytes());
        	
            conn=DriverManager.getConnection(url, user, password);  
            //if (!conn.isClosed()) {  
            //    System.out.println("Line MYsql Success!");  } 
            
            fout.write("have connect  ok !!!!!!!".getBytes());
            
        } catch (SQLException e) {
            fout.write("False line: ".getBytes());  
            fout.write(e.getMessage().getBytes());
        }  
        fout.close();
        return conn;  
    }  
	
}  