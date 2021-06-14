package com.mycom.happyhouse.contoller;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

import com.mycom.happyhouse.dto.HouseDealDto;
import com.mycom.happyhouse.dto.HouseResultDto;
import com.mycom.happyhouse.dto.HouseSimpleDto;
import com.mycom.happyhouse.service.HouseService;

@RestController
@CrossOrigin(
		origins = "http://localhost:5500", // allowCredentials = "true" 일 경우, orogins="*" 는 X
		allowCredentials = "true", 
		allowedHeaders = "*", 
		methods = {RequestMethod.GET,RequestMethod.POST,RequestMethod.DELETE,RequestMethod.PUT,RequestMethod.HEAD,RequestMethod.OPTIONS}
)
public class HouseRestController {
	
	@Autowired
	HouseService houseService;
		
	@GetMapping("/house-deals")
	public ResponseEntity<HouseResultDto> getInfo(
			@RequestParam String key, @RequestParam String word, 
			@RequestParam int limit, @RequestParam int offset){	
		System.out.println("key and word"+key+"/"+word);
		HouseResultDto result = houseService.getTotalInfo(key, word, limit, offset);
		if(result != null) {
			return new ResponseEntity(result, HttpStatus.OK);
		}
		else {
			return new ResponseEntity(null, HttpStatus.INTERNAL_SERVER_ERROR);			
		}
	}
	
	@GetMapping("/house-deals/total")
	public ResponseEntity<List<HouseSimpleDto>> getAll(@RequestParam String key, @RequestParam String word){	
		List<HouseSimpleDto> result = houseService.getAll(key,word);
		if(result != null) {
			return new ResponseEntity(result, HttpStatus.OK);
		}
		else {
			return new ResponseEntity(null, HttpStatus.INTERNAL_SERVER_ERROR);			
		}
	}
	@GetMapping("/house-deals/dealinfo")
	public ResponseEntity<List<HouseDealDto>> getDealInfo(@RequestParam int dealNo){
		System.out.println("no"+dealNo);
		List<HouseDealDto> result = houseService.getDealInfo(dealNo);
		if(result != null) {
			return new ResponseEntity(result, HttpStatus.OK);
		}
		else {
			return new ResponseEntity(null, HttpStatus.INTERNAL_SERVER_ERROR);			
		}		
	}
}
