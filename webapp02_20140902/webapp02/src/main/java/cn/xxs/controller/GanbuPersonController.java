package cn.xxs.controller;

import java.io.IOException;
import java.io.PrintWriter;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.datasource.DataSourceTransactionManager;
import org.springframework.stereotype.Controller;
import org.springframework.transaction.TransactionDefinition;
import org.springframework.transaction.TransactionStatus;
import org.springframework.transaction.support.DefaultTransactionDefinition;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.servlet.ModelAndView;

import cn.xxs.entity.Person;
import cn.xxs.entity.PersonCondition;
import cn.xxs.entity.SelectItem;
import cn.xxs.entity.User;
import cn.xxs.paging.PageContext;
import cn.xxs.service.ConstantsService;
import cn.xxs.service.GanbuService;
import cn.xxs.service.PersonService;
import cn.xxs.service.SequenceService;
import cn.xxs.utility.BaseController;

@Controller
@RequestMapping("/")
public class GanbuPersonController extends BaseController {
	@Autowired
	private GanbuService ganbuService;
	@Autowired
	private DataSourceTransactionManager transactionManager;
	@Autowired
	private ConstantsService constantService;
	@Autowired
	private PersonService personService;
	@Autowired
	private SequenceService sequenceService;
	private static Logger log = Logger.getLogger(ShemiPersonController.class);

	@RequestMapping(value = "bmgbjbqkwh")
	public ModelAndView GanbuLoad(HttpServletRequest request, HttpServletResponse response, Model model) {
		List<SelectItem> sfzzs=constantService.getSelectListByType("是否专职");
		List<SelectItem> xbs=constantService.getSelectListByType("性别");
		List<SelectItem> zzmms=constantService.getSelectListByType("政治面貌_1");
	
		model.addAttribute("shifouzhuanzhiList", sfzzs);
		model.addAttribute("xingbieList", xbs);
		model.addAttribute("zhengzhimianmaoList", zzmms);
		return new ModelAndView("secretleader/secretLeaderList");
	}
	//ganbu人员添加
			@RequestMapping(value="bmgbxj")
			public ModelAndView addGanbu(HttpSession session, Model model) {
				List<SelectItem> minzus=constantService.getSelectListByType("民族");
				List<SelectItem> zhiwus=constantService.getSelectListByType("职务");
				List<SelectItem> xuelis=constantService.getSelectListByType("学历");
				List<SelectItem> zhengzhimianmaos=constantService.getSelectListByType("政治面貌_1");
				List<SelectItem> xingzhengjibies=constantService.getSelectListByType("行政级别");
				List<SelectItem> jishuzhichengs=constantService.getSelectListByType("技术职称");
				List<SelectItem> shemidengjis=constantService.getSelectListByType("涉密等级");
				List<SelectItem> renyuanleixings=constantService.getSelectListByType("人员类型");
				List<SelectItem> xingbies=constantService.getSelectListByType("性别");
				List<SelectItem> yaohaibumenrenyuans=constantService.getSelectListByType("是否要害部门人员");
				List<SelectItem> shifouzhuanzhis=constantService.getSelectListByType("是否专职");
				//
				model.addAttribute("minzuList", minzus);
				model.addAttribute("zhiwuList", zhiwus);
				model.addAttribute("xueliList", xuelis);
				model.addAttribute("zhengzhimianmaoList", zhengzhimianmaos);
				model.addAttribute("xingzhengjibieList", xingzhengjibies);
				model.addAttribute("jishuzhichengList", jishuzhichengs);
				model.addAttribute("shemidengjiList", shemidengjis);
				model.addAttribute("renyuanleixingList", renyuanleixings);
				model.addAttribute("xingbieList", xingbies);
				model.addAttribute("yaohaibumenrenyuanList", yaohaibumenrenyuans);
				model.addAttribute("shifouzhuanzhiList", shifouzhuanzhis);
				model.addAttribute("leixing","保密干部添加");
				return new ModelAndView("person/personAdd");
			}
			

			//ganbu人员添加
			@RequestMapping(value="dobmgbtj",method=RequestMethod.POST)
			@ResponseBody
			public void addGanbuInfo(HttpSession session,HttpServletRequest request, HttpServletResponse response, @ModelAttribute Person person) throws IOException {
				response.setContentType("text/html;charset=UTF-8");
				PrintWriter out=response.getWriter();
				log.debug("------------------------------------------");
				User user = (User)session.getAttribute("user");
				log.debug("user:" + user+"orgid:"+user.getOrgid());
				log.debug("person:"+person.getName()+","+person.getMinzu());
				DefaultTransactionDefinition def = new DefaultTransactionDefinition();
				def.setPropagationBehavior(TransactionDefinition.PROPAGATION_REQUIRED);
				TransactionStatus status = transactionManager.getTransaction(def);
				Map<String, Object> map = new HashMap<String, Object>();
				try {
					if(!person.getName().isEmpty()) {				
						int id=sequenceService.getSequence("person");
						person.setId(id);
						person.setOrgid(user.getOrgid());
						log.debug("id:"+id+",orgid:"+user.getOrgid());
						personService.insertPerson(person);
						ganbuService.insertr_ganbu_person(user.getOrgid(),id);
					}
				} catch(Exception e) {
					transactionManager.rollback(status);
					map.put("errMsg", e.getMessage());
					out.print(map.toString());
					return ;
				}
				
				transactionManager.commit(status);
				map.put("errMsg", "");
				out.print("success");
				return ;
			}
			
						//ganbu人员添加
			@SuppressWarnings("deprecation")
			@RequestMapping(value="bmgbAdd")
			@ResponseBody
			public void smryAdd(HttpSession session,HttpServletRequest request, HttpServletResponse response, @ModelAttribute Person person) throws IOException {
				response.setContentType("text/html;charset=UTF-8");
				PrintWriter out=response.getWriter();
				log.debug("------------------------------------------");
				User user = (User)session.getAttribute("user");
				log.debug("user:" + user+"orgid:"+user.getOrgid());
				log.debug("person:"+person.getName()+","+person.getMinzu());
				DefaultTransactionDefinition def = new DefaultTransactionDefinition();
				def.setPropagationBehavior(TransactionDefinition.PROPAGATION_REQUIRED);
				TransactionStatus status = transactionManager.getTransaction(def);
				int id = (int)session.getAttribute("id");
				log.debug("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"+id);
				ganbuService.insertr_ganbu_person(user.getOrgid(), id);
				Map<String, Object> map = new HashMap<String, Object>();
//				try {
//					if(!person.getName().isEmpty()) {				
//						int id=sequenceService.getSequence("person");
//						person.setId(id);
//						person.setOrgid(user.getOrgid());
//						log.debug("id:"+id+",orgid:"+user.getOrgid());
//						personService.insertPerson(person);
//						shemiService.insertr_shemi_person(user.getOrgid(), id);
//					}
//				} catch(Exception e) {
//					transactionManager.rollback(status);
//					map.put("errMsg", e.getMessage());
//					out.print(map.toString());
//					return ;
//				}
//				
				transactionManager.commit(status);
				map.put("errMsg", "");
				out.print("success");
				session.removeValue("id");
				return ;
			}

			//ganbu人员修改
			@RequestMapping(value="bmgbxg")
			public ModelAndView updateGanbu(HttpSession session, Model model,String id) {
				log.debug("++++++++++++++++++++++++  id:"+id);
				if(id!=null && id!="")
				{
					//将人员id信息存在session中
					session.setAttribute("personId", id);
					Person person=personService.getPersonById(Integer.parseInt(id));
					if(person!=null)
					{
						log.debug("minzu:"+person.getMinzu()+",name:"+person.getName());
						model.addAttribute("person", person);
					}
				}
				//
				List<SelectItem> minzus=constantService.getSelectListByType("民族");
				List<SelectItem> zhiwus=constantService.getSelectListByType("职务");
				List<SelectItem> xuelis=constantService.getSelectListByType("学历");
				List<SelectItem> zhengzhimianmaos=constantService.getSelectListByType("政治面貌_1");
				List<SelectItem> xingzhengjibies=constantService.getSelectListByType("行政级别");
				List<SelectItem> jishuzhichengs=constantService.getSelectListByType("技术职称");
				List<SelectItem> shemidengjis=constantService.getSelectListByType("涉密等级");
				List<SelectItem> renyuanleixings=constantService.getSelectListByType("人员类型");
				//
				model.addAttribute("minzuList", minzus);
				model.addAttribute("zhiwuList", zhiwus);
				model.addAttribute("xueliList", xuelis);
				model.addAttribute("zhengzhimianmaoList", zhengzhimianmaos);
				model.addAttribute("xingzhengjibieList", xingzhengjibies);
				model.addAttribute("jishuzhichengList", jishuzhichengs);
				model.addAttribute("shemidengjiList", shemidengjis);
				model.addAttribute("renyuanleixingList", renyuanleixings);
				
				return new ModelAndView("secretleader/secretLeaderModify");
			}
			
			//ganbu人员修改
			@RequestMapping(value="dobmgbxg11")
			@ResponseBody
			public void updateGanbuInfo(HttpSession session,HttpServletRequest request, HttpServletResponse response, @ModelAttribute Person person) throws IOException {
				response.setContentType("text/html;charset=UTF-8");
				PrintWriter out=response.getWriter();
				log.debug("////////------------------------------------------");
				log.debug("person:"+person.getName()+","+person.getMinzu());
				DefaultTransactionDefinition def = new DefaultTransactionDefinition();
				def.setPropagationBehavior(TransactionDefinition.PROPAGATION_REQUIRED);
				TransactionStatus status = transactionManager.getTransaction(def);
				Map<String, Object> map = new HashMap<String, Object>();
				try {
					//更新信息
					if(!person.getName().isEmpty()) {	
						person.setId(Integer.parseInt( (String) session.getAttribute("personId")));
						personService.updatePerson(person);
					}
				} catch(Exception e) {
					transactionManager.rollback(status);
					map.put("errMsg", e.getMessage());
					out.print(map.toString());
					return ;
				}
				session.removeAttribute("personId");
				transactionManager.commit(status);
				map.put("errMsg", "");
				out.print("success");
				return ;
			}

	@RequestMapping(value="bmgbcx", method=RequestMethod.POST)
	@ResponseBody
	public Map<String, Object> ganbuQuery(HttpSession session, @ModelAttribute PersonCondition personInfo, int page, int rows) throws IOException {
		User user = (User)session.getAttribute("user");
		log.debug("user:" + user);
		log.debug("xxxxxxxxxxxxxxxxxxxxxxxx" + personInfo.getSuoshubumen());
		PageContext pageContext = PageContext.getContext();
		log.debug(user.getOrgid());
		pageContext.setCurrentPage(page);
		pageContext.setPageSize(rows);
		pageContext.setPagination(true);
		
		personInfo.setOrgid(user.getOrgid());
		List<Map<String, Object>> persons = ganbuService.selectGanbuPersonPage(personInfo);
		List<SelectItem> rylxs=constantService.getSelectListByType("人员类型");
		List<SelectItem> sfyhbmrys=constantService.getSelectListByType("是否要害部门人员");
		List<SelectItem> sfzzs=constantService.getSelectListByType("是否专职");
		List<SelectItem> jszcs=constantService.getSelectListByType("技术职称");
		List<SelectItem> xzjbs=constantService.getSelectListByType("行政级别");
		List<SelectItem> zws=constantService.getSelectListByType("职务");
		List<SelectItem> xbs=constantService.getSelectListByType("性别");
		List<SelectItem> xls=constantService.getSelectListByType("学历");
		List<SelectItem> zzmms=constantService.getSelectListByType("政治面貌_1");
		List<SelectItem> mzs=constantService.getSelectListByType("民族");
		List<SelectItem> smdjs=constantService.getSelectListByType("涉密等级");
	
		
		for(Map<String, Object> e: persons) {
		log.debug("aaaaaaaaaaaaaaaaaaaaaaaaaaaaa"+(int)e.get("xingzhengjibie"));
			e.put("renyuanleixing", rylxs.get((int)e.get("renyuanleixing")-1).getName());
			e.put("yaohaibumengrenyuan", sfyhbmrys.get((int)e.get("yaohaibumengrenyuan")-1).getName());
			e.put("shifouzhuanzhi", sfzzs.get((int)e.get("shifouzhuanzhi")-1).getName());
			e.put("jishuzhicheng", jszcs.get((int)e.get("jishuzhicheng")-1).getName());
			e.put("zhiwu", zws.get((int)e.get("zhiwu")-1).getName());
			e.put("xingbie", xbs.get((int)e.get("xingbie")-1).getName());
			e.put("xingzhengjibie", xzjbs.get((int)e.get("xingzhengjibie")).getName());
			e.put("xueli", xls.get((int)e.get("xueli")-1).getName());
			e.put("zhengzhimianmao", zzmms.get((int)e.get("zhengzhimianmao")-1).getName());
			e.put("minzu", mzs.get((int)e.get("minzu")-1).getName());
			e.put("shemidengji", smdjs.get((int)e.get("shemidengji")-1).getName());
		}
		

		Map<String, Object> json = new HashMap<String, Object>();
		json.put("total", pageContext.getTotalRows());
		json.put("pages", (pageContext.getTotalRows() + rows - 1) / rows);
		json.put("rows", persons);

		return json;
	}

	@RequestMapping(value = "bmgbsc", method = RequestMethod.POST)
	@ResponseBody
	public Map<String, Object> delete(HttpSession session,@RequestBody PersonCondition condition) {
		DefaultTransactionDefinition def = new DefaultTransactionDefinition();
		def.setPropagationBehavior(TransactionDefinition.PROPAGATION_REQUIRED);
		TransactionStatus status = transactionManager.getTransaction(def);
		User user = (User)session.getAttribute("user");		
		condition.setOrgid(user.getOrgid());
		Map<String, Object> map = new HashMap<String, Object>();
	
		try {
			ganbuService.deleteGanbuPerson(condition.getOrgid(), condition.getIds());
		} catch(Exception e) {
			map.put("message", "删除失败");
			transactionManager.rollback(status);
			return map;
		}
		
		transactionManager.commit(status);
		
		map.put("message", "删除成功"); 

		return map;
	}


}