package com.mycom.happyhouse.interceptor;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import org.springframework.stereotype.Component;
import org.springframework.web.servlet.handler.HandlerInterceptorAdapter;

import com.google.gson.Gson;
import com.google.gson.JsonObject;
import com.mycom.happyhouse.dto.UserDto;

@Component
public class LoginInterceptor extends HandlerInterceptorAdapter {
   
   @Override
   public boolean preHandle(HttpServletRequest request, HttpServletResponse response, Object handler)
         throws Exception {
      
	   if(request.getMethod().equals("OPTIONS")) {
		   return true;
	   }
//      String async = request.getHeader("async");
//      
//      HttpSession session = request.getSession();
//      UserDto userDto = (UserDto) session.getAttribute("userinfo");
//      
//      if(userDto == null) {
//         if("true".equals(async)) {
//            System.out.println(async);
//            Gson gson = new Gson();
//            
//            JsonObject jsonObject = new JsonObject();
//            jsonObject.addProperty("nextLink", "/users/login.do");
//            
//            String jsonStr = gson.toJson(jsonObject);
//            response.getWriter().write(jsonStr);
//            
//         } else {
//            response.sendRedirect(request.getContextPath() + "/users/login.do");
//         }
//         
//         return false;
//      }
      return super.preHandle(request, response, handler);
   }
   
}