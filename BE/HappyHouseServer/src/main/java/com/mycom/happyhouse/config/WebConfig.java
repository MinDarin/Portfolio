package com.mycom.happyhouse.config;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Configuration;
import org.springframework.web.servlet.config.annotation.InterceptorRegistry;
import org.springframework.web.servlet.config.annotation.ResourceHandlerRegistry;
import org.springframework.web.servlet.config.annotation.WebMvcConfigurer;

import com.mycom.happyhouse.interceptor.LoginInterceptor;

@Configuration
public class WebConfig  implements WebMvcConfigurer {

    @Autowired
    LoginInterceptor loginInterceptor;

    @Override
    public void addInterceptors(InterceptorRegistry registry) {
        registry.addInterceptor(loginInterceptor)
            .addPathPatterns("/**")
            .excludePathPatterns("/")
            .excludePathPatterns("/users/**")
            .excludePathPatterns("*.jsp")
            .excludePathPatterns("/css/**", "/fonts/**", "/js/**", "/img/**");
        	
    }
    
//    @Override
//	public void addInterceptors(InterceptorRegistry registry) {
//		registry.addInterceptor(loginInterceptor)
//		.addPathPatterns("/**")
//		.excludePathPatterns(
//				"/",
//				"/index.html", 
//				"/favicon.ico", 
//				"/css/**", 
//				"/js/**",
//				"/login/**", 
//				"/register/**"); // static folder
//	}
}