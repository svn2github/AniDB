<tmpl_var name="global_user">
<tmpl_loop name="loop_anime">
	<tmpl_loop name="loop_ep">
		<tmpl_loop name="loop_file">
			<tmpl_if name="data_file_length">
				<tmpl_if name="data_file_viewdate">
					[
						<tmpl_var name="data_file_viewdate">
						<tmpl_var name="data_file_length">
					]
				</tmpl_if>
			</tmpl_if>
		</tmpl_loop>
	</tmpl_loop>
</tmpl_loop>
