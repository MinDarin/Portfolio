package com.mycom.happyhouse.contoller;

import javax.servlet.http.HttpSession;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

import com.mycom.happyhouse.dto.StoreParamDto;
import com.mycom.happyhouse.dto.StoreResultDto;
import com.mycom.happyhouse.dto.UserDto;
import com.mycom.happyhouse.service.StoreService;

@CrossOrigin(
		origins = "http://localhost:5500", // allowCredentials = "true" 일 경우, orogins="*" 는 X
		allowCredentials = "true", 
		allowedHeaders = "*", 
		methods = {RequestMethod.GET,RequestMethod.POST,RequestMethod.DELETE,RequestMethod.PUT,RequestMethod.HEAD,RequestMethod.OPTIONS}
)
@RestController
@RequestMapping("/stores")
public class StoreRestController {

	@Autowired
	StoreService service;
	
	private static final int SUCCESS = 1;
	private static final int FAIL = -1;
	
	@GetMapping(value="")
	public ResponseEntity<StoreResultDto> getList(StoreParamDto paramDto){
		
		 StoreResultDto result = service.getList(paramDto);
		
		if(result.getResult() == SUCCESS) {
			return new ResponseEntity<StoreResultDto>(result, HttpStatus.OK);
		} else {
			return new ResponseEntity<StoreResultDto>(result, HttpStatus.INTERNAL_SERVER_ERROR);
		}
	}
	
	@GetMapping(value="/near")
	public ResponseEntity<StoreResultDto> getNearStoreList(StoreParamDto param, HttpSession session){
		
		StoreResultDto result = null;
		System.out.println(param.getHouseinfoNo());
		UserDto user = (UserDto) session.getAttribute("userinfo");
//		UserDto user = new UserDto();
//		user.setUserSeq(12);
		if(user == null)
			return new ResponseEntity<>(result, HttpStatus.UNAUTHORIZED);
		
		param.setUserSeq(user.getUserSeq());
		result = service.getNearStoreList(param);
		
		
		if(result.getResult() == SUCCESS) {
			return new ResponseEntity<StoreResultDto>(result, HttpStatus.OK);
		} else {
			return new ResponseEntity<StoreResultDto>(result, HttpStatus.INTERNAL_SERVER_ERROR);
		}
		
		
	}
	
}
