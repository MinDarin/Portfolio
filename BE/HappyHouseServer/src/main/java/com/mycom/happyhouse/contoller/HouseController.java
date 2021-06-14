package com.mycom.happyhouse.contoller;

import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;

import com.mycom.happyhouse.PageNotPreparedException;

@Controller
public class HouseController {
	@GetMapping("/house-deals.do")
	public String getTotal(){
		return "/jsp/result";
	}
	
	@RequestMapping(value = {"/house/setfavorite","/house/mvfavorite"})
	public void notice()throws Exception {
		throw new PageNotPreparedException();
	}

}
