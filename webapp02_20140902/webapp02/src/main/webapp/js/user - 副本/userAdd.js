function checkInput() {
	var bRtn = true;
	
	if($("#id").val() == null || $("#id").val() == "") {
		$("#idMsg").html("�û�������Ϊ�գ�");
		bRtn = false;
	} else {
		$("#idMsg").html("");
	}

	if($("#name").val() == null || $("#name").val() == "") {
		$("#nameMsg").html("��������Ϊ�գ�");
		bRtn = false;
	} else {
		$("#nameMsg").html("");
	}

	if($("#name").val() == null || $("#name").val() == "") {
		$("#nameMsg").html("��������Ϊ�գ�");
		bRtn = false;
	} else {
		$("#nameMsg").html("");
	}

	var rolesNoChecked = true;
	$("input[name='roles']:checkbox").each(function(){ 
		if($(this).attr("checked")) {
			rolesNoChecked = false;
//			break;
		}
	});
	if(rolesNoChecked) {
		$("#rolesMsg").html("��Ϊ�û�ѡ���ɫ��");
	} else {
		$("#rolesMsg").html("");
	}
	
	if($("#orgName").val() != null && $("#orgName").val() != "" && $("#orgName").val() == $("#suOrgName").val()) {
		$("#suOrgNameMsg").html("�û������Ļ����������û������������ϼ���λ������ͬ��");
		bRtn = false;
	} else {
		$("#suOrgNameMsg").html("");
	}

	return bRtn;
}
