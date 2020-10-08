#pragma once

/* 正则表达式符号树  ReSymbolTree 
   1.功能 
   对正则表达式字符串进行抽象，将字符和符号通过树的形式组织起来，例如    ab|cd* 为:
	
		    &		
		a		|
			  b   c

	2.限制 简易版本仅支持  连接，选择，闭包 三种符号即   &  |  * 三种符号
	3.功能

   */

class ReSymbolTreeElement;


class ReSymbolTree
{

};

