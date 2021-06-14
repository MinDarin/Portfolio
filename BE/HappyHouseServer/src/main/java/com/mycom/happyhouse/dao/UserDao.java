package com.mycom.happyhouse.dao;

import java.sql.SQLException;
import java.util.List;

import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;

import com.mycom.happyhouse.dto.UserDto;
import com.mycom.happyhouse.dto.UserStoreCodeDto;

@Mapper
public interface UserDao {
//	회원가입
	int registerUser(UserDto userDto) throws SQLException;

//	로그인
	UserDto login(@Param("userId") String userId, @Param("userPwd") String userPwd);

//	회원정보 수정을 위한 회원의 모든 정보 얻기
	UserDto getUser(@Param("userId") String userId) throws SQLException;

//	회원정보 수정
	int modifyUser(UserDto userDto) throws SQLException;

//	회원탈퇴
	int deleteUser(@Param("userId") String userId) throws SQLException;
	
	
	int registerUserStoreCode(UserStoreCodeDto dto);
	
	List<UserStoreCodeDto> getUserStoreCodeList(UserDto user);
}
