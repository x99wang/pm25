package com.inDB;

import java.util.Date;
import java.sql.*;
import java.text.SimpleDateFormat;

import org.json.JSONException;
import org.json.JSONObject;

public class InsertValue
{
	private JSONObject obj;
	Connection con;
	
	java.sql.Date sdate;
	java.sql.Time stime;
	
	int pmID,coorID;
	
	public InsertValue(){}
	public InsertValue(JSONObject data,String url,String user,String pw)
	{
		this.obj = data;
		
		DBCon ud = new DBCon(url,user,pw);
		this.con = ud.getDataBaseConnection();
		
		sdate = this.getSQLDate();
		stime = this.getSQLTime();
		
		pmID = -1;
		coorID = -1;
	}

	public boolean InsertSql()
	{
		if(pmInfo()&&coordinatesInfo()&&pmData())
		{
			try 
			{	con.close();	}
			catch (SQLException e)
			{	e.printStackTrace();	}
			
			return true;
		}
		
		return false;
    }
	
	private boolean pmInfo()
	{
		PreparedStatement ps=null;
		
		/*Statement sm=null;
		try
		{	sm = con.createStatement();	}
		catch(SQLException e1)
		{	e1.printStackTrace();	}*/
		
        try
        {
			String sql_pmInfo="insert into "
					+ "pmInfo(ch2o,co,pm25,pm10,temp,humid,insertTime,insertDate) "
					+"values(?,?,?,?,?,?,?,?)";
		
			ps = con.prepareStatement(sql_pmInfo);
			
			inPmInfo(ps);
			
			//ps.executeUpdate();//Can't have stats of Insert;
			
			int result=ps.executeUpdate();
			
			if(result>0)
			{
				String maxID="SELECT MAX(id) FROM pmInfo";
				ps=con.prepareStatement(maxID);
				ResultSet rs=ps.executeQuery();
				if(rs.next())
					pmID=rs.getInt(1);
				
				rs.close();
				ps.close();
                return true;
            }
        }
        catch(Exception e)
        {	e.printStackTrace();	}

        System.out.println("pmInfo fail ...");
        return false;
	}
    private void inPmInfo(PreparedStatement ps)
    {
     	try
     	{
     		//ps.setDouble(1,Integer.parseInt(obj.get("id").toString()));
			ps.setDouble(1,Double.parseDouble(obj.get("ch2o").toString()));
			ps.setDouble(2,Double.parseDouble(obj.get("co").toString()));
			ps.setDouble(3,Double.parseDouble(obj.get("pm25").toString()));
			ps.setDouble(4,Double.parseDouble(obj.get("pm10").toString()));
			ps.setDouble(5,Double.parseDouble(obj.get("temp").toString()));
			ps.setDouble(6,Double.parseDouble(obj.get("humid").toString()));
			ps.setTime(7,stime);
			ps.setDate(8,sdate);
		}
     	catch(Exception e){	System.out.println("PreparedStatement SET fail!");	}
    }
    
    private boolean coordinatesInfo()
	{
		PreparedStatement ps=null;
		
		/*Statement sm=null;
		try
		{	sm = con.createStatement();	}
		catch(SQLException e1)
		{	e1.printStackTrace();	}*/
		
        try
        {
			String sql_coorInfo="insert into "
					+ "coordinatesInfo(longitude,latitude,insertTime,insertDate) "
					+ "values(?,?,?,?)";
		
			ps = con.prepareStatement(sql_coorInfo);
			
			inCoorInfo(ps);
			
			//ps.executeUpdate();//Can't have stats of Insert;
			
			int result=ps.executeUpdate();
			
			if(result>0)
			{
				String maxID="SELECT MAX(id) FROM coordinatesInfo";
				ps=con.prepareStatement(maxID);
				ResultSet rs=ps.executeQuery();
				if(rs.next())
					coorID=rs.getInt(1);
				
				rs.close();				
				ps.close();
                return true;
            }
        }
        catch(Exception e)
        {	e.printStackTrace();	}

        System.out.println("coorInfo fail ...");
        return false;
	}
    private void inCoorInfo(PreparedStatement ps)
    {
     	try
     	{
     		//ps.setDouble(1,Integer.parseInt(obj.get("id").toString()));
			ps.setDouble(1,Double.parseDouble(obj.get("lng").toString()));
			ps.setDouble(2,Double.parseDouble(obj.get("lat").toString()));
			ps.setTime(3,stime);
			ps.setDate(4,sdate);
		}
     	catch(Exception e){	System.out.println("PreparedStatement SET fail!");	}
    }
    
    private boolean pmData()
 	{
 		PreparedStatement ps=null;
 		
 		/*Statement sm=null;
 		try
 		{	sm = con.createStatement();	}
 		catch(SQLException e1)
 		{	e1.printStackTrace();	}*/
 		
         try
         {
        	 
 			String sql_pmData="insert into "
 					+ "pmData(deviceID,pmID,coordinatesID,placeID) "
 					+ "values(?,?,?,?)";
 		
 			if(getPlaceID()==0)
 				sql_pmData="insert into "
 	 					+ "pmData(deviceID,pmID,coordinatesID) "
 	 					+ "values(?,?,?)";
 			
 			
 			ps = con.prepareStatement(sql_pmData);
 			
 			inPmData(ps);
 			
 			//ps.executeUpdate();//Can't have stats of Insert;
 			
 			int result=ps.executeUpdate();
 			
 			if(result>0)
 			{
 				ps.close();
                return true;
             }
         }
         catch(Exception e)
         {	e.printStackTrace();	}
         System.out.println("pmData fail ...");
         return false;
 	}
    private void inPmData(PreparedStatement ps)
    {
      	try
      	{
      		ps.setInt(1,Integer.parseInt(obj.get("id").toString()));
 			ps.setInt(2,pmID);
 			ps.setInt(3,coorID);
 			if(getPlaceID()!=0)
 				ps.setInt(4,getPlaceID());
 		}
      	catch(Exception e){	System.out.println("PreparedStatement SET fail!");	}
    }
     
    private int getPlaceID()
    {
    	int placeID = 0;
    	
    	PreparedStatement ps=null;
    	
    	String sql_place="SELECT MAX(placeID) FROM placeInfo "
    			+ "WHERE minlng<? "
    			+ "AND maxlng>? "
    			+ "AND minlat<? "
    			+ "AND maxlat>?;";
    	try 
    	{
			ps = con.prepareStatement(sql_place);

			ps.setDouble(1,Double.parseDouble(obj.get("lng").toString()) );
			ps.setDouble(2,Double.parseDouble(obj.get("lng").toString()) );
			ps.setDouble(3,Double.parseDouble(obj.get("lat").toString()) );
			ps.setDouble(4,Double.parseDouble(obj.get("lat").toString()) );
			
			ResultSet rs = ps.executeQuery();
			if(rs.next())
				placeID=rs.getInt(1);
			
			ps.close();
		} 
    	catch(NumberFormatException e) 
    	{	e.printStackTrace();	} 
    	catch(JSONException e) 
    	{	e.printStackTrace();	}
    	catch(SQLException e) 
    	{	e.printStackTrace();	}
    	
		return placeID;
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