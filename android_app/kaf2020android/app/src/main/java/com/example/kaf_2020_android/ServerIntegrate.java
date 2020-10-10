package com.example.kaf_2020_android;

import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import java.util.Objects;
import okhttp3.HttpUrl;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;

public class ServerIntegrate {
    final String server_url = "http://10.0.0.6:12345";

    /* access modifiers changed from: package-private */
    public Response sendRequest(String base_url, String endpoint, Map<String, String> params) throws IOException {
        OkHttpClient client = new OkHttpClient();
        try {
            HttpUrl.Builder urlBuilder = ((HttpUrl) Objects.requireNonNull(HttpUrl.parse(base_url + endpoint))).newBuilder();
            for (Map.Entry<String, String> entry : params.entrySet()) {
                urlBuilder.addQueryParameter(entry.getKey(), entry.getValue());
            }
            Response response = client.newCall(new Request.Builder().url(urlBuilder.build().toString()).build()).execute();
            return response;
        } catch (NullPointerException e) {
            return null;
        }
    }

    /* access modifiers changed from: package-private */
    public Boolean register(String username, String password, String age) throws IOException {
        Map<String, String> params = new HashMap<>();
        params.put("username", username);
        params.put("password", password);
        params.put("age", age);
        Response resp = sendRequest(server_url, "/register", params);
        return resp.code() == 200;
    }

    /* access modifiers changed from: package-private */
    public Boolean login(String username, String password) throws IOException {
        Map<String, String> params = new HashMap<>();
        params.put("username", username);
        params.put("password", password);
        Response resp = sendRequest(server_url, "/login", params);
        return resp.code() == 200;
    }
}