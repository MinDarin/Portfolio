package com.mycom.happyhouse.contoller;

import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.ControllerAdvice;
import org.springframework.web.bind.annotation.ExceptionHandler;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseStatus;
import org.springframework.web.servlet.NoHandlerFoundException;

@ControllerAdvice
public class ExceptionController {

//	@ExceptionHandler(value = Exception.class)
//	public String errorHandler(Exception e){		
//		return "jsp/error/error500";
//	}
//	
//	@ExceptionHandler(value = NoHandlerFoundException.class)
//	@ResponseStatus(value = HttpStatus.NOT_FOUND)
//	public String NotFound(NoHandlerFoundException nhfe) {
//		return "jsp/error/error404";		
//	}
	
	// ajax 요청에 대한 exception 처리
	// Transaction 테스트 경우 ajax 요청 -> NullPointer 오류가 위 method 가 실행되면 error page html 이 넘어옴. 아래의 것으로 변경
	@ExceptionHandler(Exception.class)
	public ResponseEntity<String> handleErrorResponseEntity(Exception e) {
		e.printStackTrace();
		return new ResponseEntity<String>("ajax_global_exception", HttpStatus.INTERNAL_SERVER_ERROR);
	}
}
