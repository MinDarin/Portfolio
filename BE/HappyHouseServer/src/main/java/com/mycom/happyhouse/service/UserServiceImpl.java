package com.mycom.happyhouse.service;

import java.sql.SQLException;
import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.mycom.happyhouse.dao.UserDao;
import com.mycom.happyhouse.dto.UserDto;
import com.mycom.happyhouse.dto.UserParamDto;
import com.mycom.happyhouse.dto.UserStoreCodeDto;

@Service
public class UserServiceImpl implements UserService {
	
	@Autowired
	UserDao dao;
		
	@Override
	public int registerUser(UserParamDto param) throws Exception {
		
		// 유저등록
		dao.registerUser(param.getUser());
		
		UserStoreCodeDto stcDto = new UserStoreCodeDto();
		stcDto.setUserSeq(param.getUser().getUserSeq());
		// 주변상가 선호 카테고리 등록
		List<Integer> stcList = param.getStoreCodeList();
		if(stcList.size() != 0) {
			for(Integer userStoreCode : stcList) {
				stcDto.setStoreCode(userStoreCode);
				dao.registerUserStoreCode(stcDto);
			}
		} else {	
			for(int storeCode = 10; storeCode<=12; storeCode++) {
				stcDto.setStoreCode(storeCode);
				dao.registerUserStoreCode(stcDto);
			}
		}

		return 1;
	}

	@Override
	public UserDto login(String userId, String userPwd) {
		return dao.login(userId, userPwd);
	}

	@Override
	public UserDto getUser(String userId) throws SQLException {
		return dao.getUser(userId);
	}

	@Override
	public int modifyUser(UserDto userDto) throws SQLException {
		return dao.modifyUser(userDto);
	}

	@Override
	public int deleteUser(String userId) throws SQLException{
		return dao.deleteUser(userId);
		
	}

	@Override
	public int registerUserStoreCode(List<UserStoreCodeDto> list) {
		
		for(UserStoreCodeDto dto : list) {
			dao.registerUserStoreCode(dto);
		}
		
		return 1;
	}

	@Override
	public List<UserStoreCodeDto> getUserStoreCodeList(UserDto user) {
		return dao.getUserStoreCodeList(user);
	}

}
