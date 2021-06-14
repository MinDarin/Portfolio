package com.mycom.happyhouse.contoller;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

import com.mycom.happyhouse.dto.CodeDto;
import com.mycom.happyhouse.service.CodeService;

@CrossOrigin(
		origins = "http://localhost:5500", // allowCredentials = "true" 일 경우, orogins="*" 는 X
		allowCredentials = "true", 
		allowedHeaders = "*", 
		methods = {RequestMethod.GET,RequestMethod.POST,RequestMethod.DELETE,RequestMethod.PUT,RequestMethod.HEAD,RequestMethod.OPTIONS}
)
@RestController
@RequestMapping(value="/codes")
public class CodeController {

	@Autowired
	CodeService service;
	
	
	@GetMapping("")
	public ResponseEntity<List<CodeDto>> getList(int groupCode){
		
		List<CodeDto> list = service.getList(groupCode);
		
		if(list != null) {
			return new ResponseEntity<List<CodeDto>>(list, HttpStatus.OK);
		}else {
			return new ResponseEntity<List<CodeDto>>(list, HttpStatus.NOT_FOUND);
		}
		
	}
}
