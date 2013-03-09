$(document).ready(function()
{
	$.ajaxSetup({
		error: function (x, status, error) {
			alert("An error occurred: " + status + "\nError: " + error + "\nStatus: " + x.statusText);
		},
		beforeSend: function(xhr){xhr.setRequestHeader('X_LCONTROL', 'x-no-cache');},
	});

	$.ajax({
		'async': true,
		'global': false,
		'url': 'http://anidb.mine.nu/anidb/animedb.pl?show=json&action=get-tags&reltb=animetb&relid=1',
		'dataType': "json",
		'success': function (data) {
			var item  = ' \
			<div id="tag_add" class="g_section tag_add"> \
				<div id="taggroups" class="g_section taggroups"> \
					<span class="taggroup"></span> \
				</div> \
				<div id="taglist" class="g_section taglist"> \
					<div class="g_bubble tags hide"> \
						<h4 class="parent"><span></span></h4> \
						<span class="tag"></span> \
					</div> \
				</div> \
			</div>';

			$("#tagaddbox").after(item);

			var directive = {
				'#taggroups span.taggroup' : {
					'group<-groups' : {
						'.'	: 'group.name',
						'.@id' : 'group.tagid'
					}
				}
			};

			$('#tag_add').render(data, directive);

			var directive2 = {
				'#taglist div.tags' : {
					'parent<-parents' : {
						'h4@id'			 : 'parent.tagid',
						'h4.parent span' : 'parent.name',
						'span.tag' : {
							'child<-parent.childs' : {
								'.'   : 'child.name',
								'@id' : 'child.tagid'
							}
						}
					}
				}
			};

			$('#tag_add').render(data, directive2);
		}
	});
});