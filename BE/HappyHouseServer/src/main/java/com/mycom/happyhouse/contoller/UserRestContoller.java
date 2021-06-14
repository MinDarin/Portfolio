package com.mycom.happyhouse.contoller;

import java.util.List;

import javax.servlet.http.HttpSession;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Service;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.PutMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

import com.mycom.happyhouse.dto.UserDto;
import com.mycom.happyhouse.dto.UserParamDto;
import com.mycom.happyhouse.dto.UserStoreCodeDto;
import com.mycom.happyhouse.service.UserService;

@CrossOrigin(
		origins = "http://localhost:5500", // allowCredentials = "true" 일 경우, orogins="*" 는 X
		allowCredentials = "true", 
		allowedHeaders = "*", 
		methods = {RequestMethod.GET,RequestMethod.POST,RequestMethod.DELETE,RequestMethod.PUT,RequestMethod.HEAD,RequestMethod.OPTIONS}
)
@RestController
@RequestMapping("/users")
public class UserRestContoller {

	@Autowired
	UserService userService;
	
	@PostMapping("/login")
	public ResponseEntity<UserDto> login(@RequestBody UserDto loginUser, HttpSession session) {
	
		UserDto user = userService.login(loginUser.getUserId(), loginUser.getUserPassword());
	
		ResponseEntity<UserDto> result;
		
		if(user != null) {
			session.setAttribute("userinfo", user);
			user.setUserPassword("");
			result = new ResponseEntity<UserDto>(user,HttpStatus.OK);
		}else {
			result = new ResponseEntity<UserDto>(user,HttpStatus.UNAUTHORIZED);
		}
		return result;
	}
	@PostMapping("/logout")
	public ResponseEntity<UserDto> logout(HttpSession session){
		
		UserDto user = null;
		ResponseEntity<UserDto> result = new ResponseEntity<>(user, HttpStatus.OK);
		
		session.invalidate();
		
		return result;
	}
	
	
	@PostMapping("")
	public ResponseEntity<Integer> registerUser(@RequestBody UserParamDto param) throws Exception{
		
		int ret = userService.registerUser(param);
		ResponseEntity<Integer> result = new ResponseEntity<Integer>(ret, HttpStatus.CREATED);
		return result;
	}
	
	@DeleteMapping("/{userId}")
	public ResponseEntity<Integer> deleteUser(@PathVariable(value = "userId") String userId, HttpSession session) throws Exception{
		
		System.out.println(userId);
		int ret = userService.deleteUser(userId);
		ResponseEntity<Integer> result;
		if(ret == 1) {
			session.invalidate();
			result = new ResponseEntity<Integer>(ret, HttpStatus.OK);
		} else {
				result = new ResponseEntity<Integer>(0,HttpStatus.INTERNAL_SERVER_ERROR);
		}
		
		return result;	
	}
	
	@PutMapping("/{userId}")
	public ResponseEntity<Integer> updateUser(@PathVariable(value = "userId") String userId, @RequestBody UserDto userDto, HttpSession session) throws Exception{
		
		int ret = userService.modifyUser(userDto);
		
		ResponseEntity<Integer> result;
		if(ret == 1) {
			session.setAttribute("userinfo", userDto);
			result = new ResponseEntity<Integer>(ret, HttpStatus.OK);
		} else {
			result = new ResponseEntity<Integer>(0,HttpStatus.INTERNAL_SERVER_ERROR);
		}
		return result;
	}
	
	@GetMapping("/{userSeq}/store-codes")
	public ResponseEntity<List<UserStoreCodeDto>> getUserStoreCodeList(
			@PathVariable(value="userSeq") int userSeq,
			HttpSession session){
			
			List<UserStoreCodeDto> list = null;
			UserDto user = (UserDto) session.getAttribute("userinfo");
			if(user != null && user.getUserSeq() == userSeq) {
				
				list = userService.getUserStoreCodeList(user);
				return new ResponseEntity<List<UserStoreCodeDto>>(list, HttpStatus.OK);
				
			}else {
				return new ResponseEntity<List<UserStoreCodeDto>>(list, HttpStatus.UNAUTHORIZED);
			}
	}

}
