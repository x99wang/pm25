import java.io.IOException;  

import net.sf.json.JSONObject;   
  
import org.apache.http.HttpResponse;  
import org.apache.http.client.ClientProtocolException;  
import org.apache.http.client.HttpClient;  
import org.apache.http.client.methods.HttpPost;  
import org.apache.http.entity.StringEntity;  
import org.apache.http.impl.client.DefaultHttpClient;  
import org.apache.http.util.EntityUtils;  

  
public class Client {  
    public void HttpPostData() {    
          try {   
	              HttpClient httpclient = new DefaultHttpClient();    
	              String uri = "http://139.199.153.220:8080/resJson/pmUpdata/Insert";   
	              HttpPost httppost = new HttpPost(uri);     
	              //���httpͷ��Ϣ   
	              /*httppost.addHeader("Authorization", "your token"); //��֤token   
	              httppost.addHeader("Content-Type", "application/json");   
	              httppost.addHeader("User-Agent", "imgfornote");*/    
	              JSONObject obj = new JSONObject();  
	              obj.put("co", "0.23");   
	              obj.put("pm25", "58");   
	              System.out.println("obj put secc");
	              httppost.setEntity(new StringEntity(obj.toString()));
	              System.out.println("post set secc");       
	              HttpResponse response;    
	              //����post
	              response = httpclient.execute(httppost);
	              System.out.println("client execut secc");
	              //����״̬�룬����ɹ���������    
	              int code = response.getStatusLine().getStatusCode();    
	              System.out.println(code+"code");  
	              if (code == 200) {    
	                  //String rev = EntityUtils.toString(response.getEntity());//����json��ʽ�� {"id": "","name": ""}   
	
	        
	                  //obj= JSONObject.fromObject(rev);  
	                
	                  //User user = (User)JSONObject.toBean(obj,User.class);  
	                  //System.out.println("��������==="+rev);  
	            	  
	            	  System.out.println("ok");
	              }   
              } catch (ClientProtocolException e) {   
                  e.printStackTrace();  
              } catch (IOException e) {    
                  e.printStackTrace();  
              } catch (Exception e) {   
                  e.printStackTrace();  
              }   
    }
    public static void main(String[]args)
    {
    	Client cl = new Client();
    	cl.HttpPostData();
    }
}  