package com.mycom.happyhouse.contoller;

import javax.servlet.http.HttpSession;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;

@Controller
@RequestMapping("/users")
public class UserController {

	@RequestMapping("/login.do")
	public String mvLogin() {
		return "jsp/login";
	}
	
	@RequestMapping("/logout")
	public String logout(HttpSession session) {
		
		session.invalidate();
		
		return "jsp/index";
	}
	
	@RequestMapping("/info.do")
	public String mvUserInfo() {
		
		return "jsp/userinfo";
	}
}
