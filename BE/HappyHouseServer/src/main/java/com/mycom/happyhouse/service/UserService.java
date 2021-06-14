package com.mycom.happyhouse.service;

import java.sql.SQLException;
import java.util.List;

import com.mycom.happyhouse.dto.UserDto;
import com.mycom.happyhouse.dto.UserParamDto;
import com.mycom.happyhouse.dto.UserStoreCodeDto;

public interface UserService {
//	회원가입
	int registerUser(UserParamDto param) throws Exception;
//	로그인
	UserDto login(String userId, String userPwd);
//	회원정보 수정을 위한 회원의 모든 정보 얻기
	UserDto getUser(String userId) throws SQLException;
//	회원정보 수정
	int modifyUser(UserDto userDto) throws SQLException;
//	회원탈퇴
	int deleteUser(String userId) throws SQLException;
	
	int registerUserStoreCode(List<UserStoreCodeDto> dto);
	
	List<UserStoreCodeDto> getUserStoreCodeList(UserDto user);
}
