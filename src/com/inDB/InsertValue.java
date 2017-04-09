package com.inDB;

import java.util.Date;
import java.sql.*;
import java.text.SimpleDateFormat;

import org.json.JSONObject;

public class InsertValue
{
	private JSONObject obj;
	Connection con;
	//Connection con2;
	Statement sm;
	//Statement sm2;
	public InsertValue(){}
	public InsertValue(JSONObject realvalue,String url,String user,String pw)
	{
		this.obj = realvalue;
		try{
			upData ud = new upData(url,user,pw);
			//upData2 ud2 = new upData2();
			this.con = ud.getDataBaseConnection();
			//this.con2 = ud2.getDataBaseConnection();
			this.sm = con.createStatement();
			//this.sm2 = con2.createStatement();
		}catch(Exception e){}
	}
/*	public void doInsert()
	{
		String v1 = rv.getWindSpeed() + "";
		String v2 = rv.getRainfall() + "";
		String v3 = rv.getTemperature() + "";
		String v4 = rv.getAirPressure() + "";
		String v5 = rv.getRadiation() + "";
		String v6 = rv.getWindDirection() + "";
		String v7 = rv.getHumidity() + "";
		String v ="INSERT INTO meteorological VALUES(NULL,'"
					+ v1 + "','"
					+ v2 + "','"
					+ v3 + "','"
					+ v4 + "','"
					+ v5 + "','"
					+ v6 + "','"
					+ v7 + "','CURRENT_TIMESTAMP')";
		
		System.out.println(v);			
					
		try{
			
			sm.executeUpdate(v);
			
			sm.close();
			con.close();
		}catch(Exception e){}
	}*/
	public boolean InsertSql(){
         try{
				String insql="insert into pmInfo(deviceId,ch2o,co,pm25,pm10,temp,humid,insertTime,insertDate) "
				+"values(?,?,?,?,?,?,?,?,?)";
				
				//insql=""insert into meteorological(WindSpeed,Rainfall,Temperature,AirPressure,Radiantion,WindDirection,Humidity,time) values(user.getId,user.getName,user.getPassword,user.getEmail)";
				
				
				PreparedStatement ps=con.prepareStatement(insql);
				//PreparedStatement ps2=con2.prepareStatement(insql);
				
				//.preparedStatement(insql);
				//PreparedStatement  ps=(PreparedStatement) conn.prepareStatement(insql);
				
				in(ps);
				//in(ps2);
				
				//ps.executeUpdate();//Can't have stats of Insert;
				
				
				int result=ps.executeUpdate();
				//int result2=ps2.executeUpdate();
				
				if(result>0)
				{
					ps.close();
					con.close();
					
					//ps2.close();
					//con2.close();
					
                    return true;
                }
            }catch(Exception e){
             e.printStackTrace();
         }
         return false;
     }
    public void in(PreparedStatement ps)
     {
     	try
     	{
     		ps.setDouble(1,Integer.parseInt(obj.get("id").toString()));
			ps.setDouble(2,Double.parseDouble(obj.get("ch2o").toString()));
			ps.setDouble(3,Double.parseDouble(obj.get("co").toString()));
			ps.setDouble(4,Double.parseDouble(obj.get("pm25").toString()));
			ps.setDouble(5,Double.parseDouble(obj.get("pm10").toString()));
			ps.setDouble(6,Double.parseDouble(obj.get("temp").toString()));
			ps.setDouble(7,Double.parseDouble(obj.get("humid").toString()));
			ps.setTime(8,getSQLTime());
			ps.setDate(9,getSQLDate());
		}catch(Exception e){}
     }
    public static java.sql.Date getSQLDate()
     {
     	java.sql.Date sqlDate = java.sql.Date.valueOf(getStringDateShort());
     	return sqlDate;
     }
    public static java.sql.Time getSQLTime()
     {
   	java.sql.Time time = java.sql.Time.valueOf(getTimeShort());  
   		return time;
     }
 	public static String getStringDateShort() 
	{
	    Date currentTime = new Date();
	    SimpleDateFormat formatter = new SimpleDateFormat("yyyy-MM-dd");
	    String dateString = formatter.format(currentTime);
	    return dateString;
  	}
  	public static String getTimeShort() 
      	{
    	    SimpleDateFormat formatter = new SimpleDateFormat("HH:mm:ss");
    	    Date currentTime = new Date();
    	    String dateString = formatter.format(currentTime);
    	    return dateString;
      	}
}