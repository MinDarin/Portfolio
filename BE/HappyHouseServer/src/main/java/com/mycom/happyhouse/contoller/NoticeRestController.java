package com.mycom.happyhouse.contoller;

import javax.servlet.http.HttpSession;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.multipart.MultipartHttpServletRequest;

import com.mycom.happyhouse.dto.NoticeDto;
import com.mycom.happyhouse.dto.NoticeParamDto;
import com.mycom.happyhouse.dto.NoticeResultDto;
import com.mycom.happyhouse.dto.UserDto;
import com.mycom.happyhouse.service.NoticeService;

@CrossOrigin(
		origins = "http://localhost:5500", // allowCredentials = "true" 일 경우, orogins="*" 는 X
		allowCredentials = "true", 
		allowedHeaders = "*", 
		methods = {RequestMethod.GET,RequestMethod.POST,RequestMethod.DELETE,RequestMethod.PUT,RequestMethod.HEAD,RequestMethod.OPTIONS}
)
@RestController
@RequestMapping("/notices")
public class NoticeRestController {
	
			@Autowired
			NoticeService service;
			
			private static final int SUCCESS = 1;
			
			@GetMapping(value="")
			public ResponseEntity<NoticeResultDto> noticeList(NoticeParamDto noticeParamDto){
				
				NoticeResultDto noticeResultDto;

				if( noticeParamDto.getSearchWord().isEmpty() ) {
					noticeResultDto = service.noticeList(noticeParamDto);
				}else {
					noticeResultDto = service.noticeListSearchWord(noticeParamDto);
				}
				
				if( noticeResultDto.getResult() == SUCCESS ) {
					return new ResponseEntity<NoticeResultDto>(noticeResultDto, HttpStatus.OK);
				}else {
					return new ResponseEntity<NoticeResultDto>(noticeResultDto, HttpStatus.INTERNAL_SERVER_ERROR);
				}
			}

			
			@GetMapping(value="/{noticeNo}")
			public ResponseEntity<NoticeResultDto> noticeDetail(@PathVariable int noticeNo, HttpSession session){

				NoticeParamDto noticeParamDto = new NoticeParamDto();
				noticeParamDto.setNoticeNo(noticeNo);
				
				NoticeResultDto noticeResultDto = service.noticeDetail(noticeParamDto);
				// 게시글 작성자와 현 사용자가 동일
				if( ((UserDto) session.getAttribute("userinfo")).getUserSeq() == noticeResultDto.getDto().getUserSeq() ) {
					noticeResultDto.setOwner(true);
				}
				
				if( noticeResultDto.getResult() == SUCCESS ) {
					return new ResponseEntity<NoticeResultDto>(noticeResultDto, HttpStatus.OK);
				}else {
					return new ResponseEntity<NoticeResultDto>(noticeResultDto, HttpStatus.INTERNAL_SERVER_ERROR);
				}		 
			}
			
			@PostMapping(value="")
			public ResponseEntity<NoticeResultDto> noticeInsert(
					NoticeDto noticeDto, 
					MultipartHttpServletRequest request) {
				
				noticeDto.setUserSeq( ((UserDto) request.getSession().getAttribute("userinfo")).getUserSeq());
				NoticeResultDto noticeResultDto = service.noticeInsert(noticeDto, request);
				
				if( noticeResultDto.getResult() == SUCCESS ) {
					return new ResponseEntity<NoticeResultDto>(noticeResultDto, HttpStatus.OK);
				}else {
					return new ResponseEntity<NoticeResultDto>(noticeResultDto, HttpStatus.INTERNAL_SERVER_ERROR);
				}		 
			}
			
			@PostMapping(value="/{noticeNo}") 
			public ResponseEntity<NoticeResultDto> noticeUpdate(
					NoticeDto noticeDto, 
					MultipartHttpServletRequest request){

				NoticeResultDto noticeResultDto = service.noticeUpdate(noticeDto, request);
				noticeDto.setUserSeq( ((UserDto) request.getSession().getAttribute("userinfo")).getUserSeq());
				
				if( noticeResultDto.getResult() == SUCCESS ) {
					return new ResponseEntity<NoticeResultDto>(noticeResultDto, HttpStatus.OK);
				}else {
					return new ResponseEntity<NoticeResultDto>(noticeResultDto, HttpStatus.INTERNAL_SERVER_ERROR);
				}		
			}
			
			@DeleteMapping(value="/{noticeNo}") 
			public ResponseEntity<NoticeResultDto> noticeDelete(@PathVariable(value="noticeNo") int noticeNo){
				NoticeResultDto noticeResultDto = service.noticeDelete(noticeNo);
			    
				if( noticeResultDto.getResult() == SUCCESS ) {
					return new ResponseEntity<NoticeResultDto>(noticeResultDto, HttpStatus.OK);
				}else {
					return new ResponseEntity<NoticeResultDto>(noticeResultDto, HttpStatus.INTERNAL_SERVER_ERROR);
				}		 
			}
	
}
